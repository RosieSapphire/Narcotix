#include "narcotix/renderer.h"
#include "narcotix/sound_engine.h"
#include "narcotix/sound.h"
#include "narcotix/font.h"
#include "narcotix/model.h"
#include "narcotix/ui.h"
#include "narcotix/bone_data.h"
#include "narcotix/helpers.h"

#include <time.h>
#include <AL/al.h>

#define WINDOW_WIDTH 				1920
#define WINDOW_HEIGHT			    1080
#define WINDOW_SIZE 				(vec2){WINDOW_WIDTH, WINDOW_HEIGHT}
#define WINDOW_ASPECT 				((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT)

#define PLAYER_MOVE_SPEED			1.8f
#define PLAYER_LERP_SPEED			10.0f
#define PLAYER_HEADBOB_LERP_SPEED	3.2f
#define PLAYER_HEADBOB_INTENSITY	0.08f
#define PLAYER_LOOK_SENSITIVITY		0.16f

#define BONG_TIMER_MAX 				5.0f
#define BONG_COOLDOWN_TIME 			40.0f

#define EPSILON 					0.001f

static ncx_renderer_t renderer;
static ncx_sound_engine_t sound_engine;

static ncx_sound_t footstep_sounds;
static ncx_sound_t lighter_sounds;
static ncx_sound_t bong_pull_sounds;
static ncx_sound_t pistol_fire_sound;
static ncx_sound_t pistol_click_sound;
static ncx_sound_t pistol_reload_sound;

static ncx_sound_t tripping_music;
static uint8_t tripping_music_playing = 0;

static ncx_font_t shagadelic_font;

static uint8_t draw_mode = 0;
static uint8_t p_press = 0;
static uint8_t q_press = 0;
static uint8_t left_click = 0;
static uint8_t right_click = 0;
static uint8_t right_click_old = 0;
static uint8_t reload_pressed = 0;

static double time_last;

static vec2 player_angle;
static vec3 player_pos_a;
static vec3 player_pos_b;
static vec3 player_right;
static vec3 player_look_pos;
static float player_headbob_timer = 0.0f;
static float player_headbob_intensity = 0.0f;
static float player_trip_intensity = 0.0f;
static float player_fov = 1.186823f;
static float player_vertical_velocity = 0.0f;
static uint8_t player_jumping = 0;
static uint8_t jump_press = 0;
static float footstep_volume = 0.0f;

static vec3 weapon_pos;
static vec2 weapon_angle;
static vec2 weapon_fire_recoil;

static ncx_model_t ncx_model_room[2];
static ncx_model_t ncx_model_pistol;
static ncx_model_t ncx_model_bong;

static ncx_texture_t room_textures[8];
static ncx_texture_t pistol_textures[8];
static ncx_texture_t bong_textures[4];
static ncx_texture_t trippy_texture;

static ncx_texture_t ncx_ui_weapon_slot_textures[3];

static mat4 matrix_projection;

static ncx_ui_element_t ncx_ui_weapon_slot;

enum {
	WEAPON_PISTOL = 0x1,
	WEAPON_BONG = 0x2,
};
static uint8_t weapon_selected = WEAPON_PISTOL;
static float bong_smoke_timer = BONG_TIMER_MAX;
static float bong_smoke_cooldown_timer = 0.0f;
static float bong_smoke_cooldown_timer_start = 0.0f;
static uint8_t bong_smoke_inhaled = 0;
static uint8_t bong_weed_count = 5;
static float bong_weed_tolerance = 0.0f;
static float bong_weed_tolerance_tar = 0.0f;

#define PISTOL_AMMO_LOADED_MAX 	18
#define PISTOL_RELOAD_TIMER_MAX 1.08f

static int8_t pistol_ammo_loaded = PISTOL_AMMO_LOADED_MAX;
static int8_t pistol_ammo_reserve = 18*2;
static float pistol_reload_timer = 0.0f;
static float pistol_fire_timer = 0.0f;

void glfw_error_callback(int code, const char *log);
void glfw_error_callback(int code, const char *log) {
	printf("AN ERROR OCCURED OH NO!: %s : %i\n", log, code);
}

#ifdef WINDOWS
int WinMain(void) {
#else
int main(void) {
#endif
	glfwSetErrorCallback((void *)&glfw_error_callback);
	ncx_renderer_create(&renderer, WINDOW_WIDTH, WINDOW_HEIGHT, 2);

	/* player initialization */
	glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, player_pos_a);
	glm_vec3_copy(player_pos_a, player_pos_b);
	glm_vec3_copy(player_pos_a, weapon_pos);
	glm_vec3_sub(weapon_pos, GLM_YUP, weapon_pos);
	glm_vec2_copy(GLM_VEC2_ZERO, player_angle);
	glm_vec2_copy((vec2){GLM_PIf, 0.0f}, weapon_angle);

	/* load textures */
	{
		const char *room_ncx_texture_paths[8] = {
			"res/models/room/floor-diffuse.png",
			"res/models/room/black.png",
			"res/models/room/wall-diffuse.png",
			room_ncx_texture_paths[1],
			"res/models/room/ceiling-diffuse.png",
			"res/models/room/ceiling-specular.png",
			"res/models/room/bruh.png",
			room_ncx_texture_paths[1]
		};

		for(uint8_t i = 0; i < 8; i += 2) {
			room_textures[i] = ncx_texture_create(room_ncx_texture_paths[i], GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1);
			room_textures[i + 1] = ncx_texture_create(room_ncx_texture_paths[i + 1], GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1);
		}

		pistol_textures[0] = ncx_texture_create("res/models/weapons/pistol/pistol_diffuse.png", GL_REPEAT, GL_NEAREST, GL_NEAREST, 0);
		pistol_textures[1] = ncx_texture_create("res/models/weapons/pistol/pistol_specular.png", GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1);
		for(uint8_t i = 0; i < 8; i += 2) {
			pistol_textures[i] = pistol_textures[0];
			pistol_textures[i + 1] = pistol_textures[1];
		}

		bong_textures[0] = pistol_textures[0];
		bong_textures[1] = pistol_textures[1];
		bong_textures[2] = bong_textures[0];
		bong_textures[3] = bong_textures[1];

		trippy_texture = ncx_texture_create("res/textures/trippy-overlay-texture.png", GL_MIRRORED_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1);
	}

	ncx_font_shader_create("res/shaders/font_vert.glsl", "res/shaders/font_frag.glsl");
	shagadelic_font = ncx_font_create("res/fonts/shagadelic.ttf");

	/* ui elements */
	ncx_ui_weapon_slot_textures[0] = ncx_texture_create("res/textures/hud/weapon-slot.png", GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1);
	ncx_ui_weapon_slot_textures[1] = ncx_texture_create("res/textures/hud/weapon-slot-pistol.png", GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1);
	ncx_ui_weapon_slot_textures[2] = ncx_texture_create("res/textures/hud/weapon-slot-bong.png", GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1);
	ncx_ui_elements_init(WINDOW_WIDTH, WINDOW_HEIGHT);
	ncx_ui_element_create(&ncx_ui_weapon_slot, (vec2){0.0333f * WINDOW_WIDTH, 0.8222f * WINDOW_HEIGHT}, (vec2){0.0667f * WINDOW_WIDTH, 0.0667f * WINDOW_WIDTH}, ncx_ui_weapon_slot_textures, 3);

	ncx_sound_engine_create(&sound_engine);


	{ /* load sounds */
		const char *footstep_sound_paths =
			"res/sounds/footsteps/0.wav\0"
			"res/sounds/footsteps/1.wav\0"
			"res/sounds/footsteps/2.wav\0"
			"res/sounds/footsteps/3.wav\0"
			"res/sounds/footsteps/4.wav\0"
			"res/sounds/footsteps/5.wav\0"
			"res/sounds/footsteps/6.wav\0"
			"res/sounds/footsteps/7.wav\0"
			"res/sounds/footsteps/8.wav\0";

		const char *lighter_sound_paths =
			"res/sounds/drugs/lighter/0.wav\0"
			"res/sounds/drugs/lighter/1.wav\0"
			"res/sounds/drugs/lighter/2.wav\0"
			"res/sounds/drugs/lighter/3.wav\0"
			"res/sounds/drugs/lighter/4.wav\0"
			"res/sounds/drugs/lighter/5.wav\0"
			"res/sounds/drugs/lighter/6.wav\0"
			"res/sounds/drugs/lighter/7.wav\0"
			"res/sounds/drugs/lighter/8.wav\0"
			"res/sounds/drugs/lighter/9.wav\0";

		const char *bong_pull_sound_paths =
			"res/sounds/drugs/bong/0.wav\0"
			"res/sounds/drugs/bong/1.wav\0"
			"res/sounds/drugs/bong/2.wav\0"
			"res/sounds/drugs/bong/3.wav\0"
			"res/sounds/drugs/bong/4.wav\0"
			"res/sounds/drugs/bong/5.wav\0";

		const char *pistol_fire_sound_path = "res/sounds/weapons/pistol/fire.wav\0";
		const char *pistol_click_sound_path = "res/sounds/weapons/pistol/click.wav\0";
		const char *pistol_reload_sound_path = "res/sounds/weapons/pistol/reload.wav\0";
		const char *tripping_music_paths =
			"res/sounds/music/tripping/0.wav\0"
			"res/sounds/music/tripping/1.wav\0"
			"res/sounds/music/tripping/2.wav\0"
			"res/sounds/music/tripping/3.wav\0"
			"res/sounds/music/tripping/4.wav\0";

		ncx_sound_create(&footstep_sounds, footstep_sound_paths, 9, 1);
		ncx_sound_create(&lighter_sounds, lighter_sound_paths, 10, 0);
		ncx_sound_create(&bong_pull_sounds, bong_pull_sound_paths, 6, 0);

		ncx_sound_create(&pistol_fire_sound, pistol_fire_sound_path, 1, 1);
		ncx_sound_create(&pistol_click_sound, pistol_click_sound_path, 1, 1);
		ncx_sound_create(&pistol_reload_sound, pistol_reload_sound_path, 1, 1);

		ncx_sound_create(&tripping_music, tripping_music_paths, 5, 0);
	}

	/* loading meshes */
	ncx_model_shader_create();
	ncx_model_room[0] = ncx_model_create("res/models/room/room-low.glb", room_textures);
	ncx_model_room[1] = ncx_model_create("res/models/room/room.glb", room_textures);
	ncx_model_pistol  = ncx_model_create("res/models/weapons/pistol/pistol.glb", pistol_textures);
	ncx_model_bong    = ncx_model_create("res/models/weapons/bong/bong.glb", bong_textures);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);

	glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	ncx_renderer_center_mouse(&renderer);

	time_last = glfwGetTime();

	srand((uint32_t)time(NULL));

	/* main loop */
	while(!glfwWindowShouldClose(renderer.window)) {
		mat4 matrix_view;
		const double time_now = glfwGetTime();
		vec2 mouse_pos;
		float time_delta;

		vec3 player_dir;
		vec3 player_headbob;
		vec3 player_pos_a_bobbed;

		/*
		{
			clock_t start = clock();
			while(clock() < start + 20000);
		}
		*/

		if(ncx_renderer_key_get_press(renderer, GLFW_KEY_ESCAPE))
			glfwSetWindowShouldClose(renderer.window, 1);

		/* fullscreen disabled for the time being because I don't know what the fuck is wrong with it */
		/*
		{
			const uint8_t fullscreen_press_old = fullscreen_press;
			fullscreen_press = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
			if(fullscreen_press && (fullscreen_press != fullscreen_press_old)) {
				renderer.is_fullscreen = !renderer.is_fullscreen;
				if(renderer.is_fullscreen) {
					glfwSetWindowMonitor(window, monitor, 0, 0, (int32_t)monitor_size[0], (int32_t)monitor_size[1], vidmode->refreshRate);
					glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
					glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
				} else {
					glfwSetWindowMonitor(window, NULL, window_position[0], window_position[1], WINDOW_WIDTH, WINDOW_HEIGHT, vidmode->refreshRate);
					glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
					glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
				}
			}
		}
		*/

		{
			const uint8_t p_press_old = p_press;
			const uint8_t q_press_old = q_press;

			p_press = ncx_renderer_key_get_press(renderer, GLFW_KEY_P);
			if(p_press && !p_press_old) {
				draw_mode = !draw_mode;
			}

			q_press = ncx_renderer_key_get_press(renderer, GLFW_KEY_Q);
			if(q_press && !q_press_old) {
				weapon_pos[1] -= 2.0f;
				weapon_selected = (uint8_t)(weapon_selected << 1) % 3;
			}
		}

		time_delta = (float)(time_now - time_last);
		time_last = time_now;

		/* camera look */
		ncx_renderer_mouse_pos_get(renderer, mouse_pos);
		mouse_pos[0] -= renderer.is_fullscreen ? renderer.monitor_size[0] / 2.0f : WINDOW_WIDTH / 2.0f;
		mouse_pos[1] -= renderer.is_fullscreen ? renderer.monitor_size[1] / 2.0f : WINDOW_HEIGHT / 2.0f;

		player_angle[0] += (float)mouse_pos[0] * PLAYER_LOOK_SENSITIVITY;
		player_angle[1] = ncx_clampf(player_angle[1] - (float)mouse_pos[1] * PLAYER_LOOK_SENSITIVITY, -89.0f, 89.0f);
		if(weapon_selected & WEAPON_PISTOL) {
			const uint8_t left_click_old = left_click;
			const uint8_t reload_pressed_old = reload_pressed;

			reload_pressed = ncx_renderer_key_get_press(renderer, GLFW_KEY_R);
			left_click = ncx_renderer_mouse_button_get(renderer, GLFW_MOUSE_BUTTON_LEFT);
			if(left_click && (left_click != left_click_old) && pistol_reload_timer < EPSILON && pistol_fire_timer < EPSILON) {
				if(pistol_ammo_loaded - 1 < 0 && pistol_ammo_reserve > 0) {
					reload_pressed = 1;
				} else {
					pistol_fire_timer = 0.12f;
					pistol_ammo_loaded--;
					if(pistol_ammo_reserve > 0 || pistol_ammo_loaded >= 0) {
						weapon_fire_recoil[1] += glm_deg(0.02f);
						ncx_sound_play(pistol_fire_sound, 0.5f, 1.0f, GLM_VEC3_ZERO, 0, 0);
						pistol_fire_sound.delay_timer = 0.0f;
					} else {
						pistol_click_sound.delay_timer = 0.0f;
						ncx_sound_play(pistol_click_sound, 0.3f, 1.0f, GLM_VEC3_ZERO, 0, 0);
						pistol_ammo_loaded = 0;
					}
				}
			}

			if(reload_pressed && (reload_pressed != reload_pressed_old) && pistol_reload_timer < EPSILON && pistol_ammo_reserve > 0 && pistol_ammo_loaded < PISTOL_AMMO_LOADED_MAX) {
				const int8_t ammo_difference = PISTOL_AMMO_LOADED_MAX - pistol_ammo_loaded;
				pistol_reload_timer = PISTOL_RELOAD_TIMER_MAX;
				pistol_reload_sound.delay_timer = 0.0f;
				ncx_sound_play(pistol_reload_sound, 0.5f, 1.0f, GLM_VEC3_ZERO, 0, 0);

				pistol_ammo_reserve -= ammo_difference;
				pistol_ammo_loaded += ammo_difference;
				if(pistol_ammo_reserve < 0) {
					pistol_ammo_loaded += pistol_ammo_reserve;
					pistol_ammo_reserve = 0;
				}
			}
		}
		pistol_fire_timer = ncx_clampf01(pistol_fire_timer - time_delta);
		pistol_reload_timer = ncx_clampf(pistol_reload_timer - time_delta, 0.0f, PISTOL_RELOAD_TIMER_MAX);
		ncx_vec2_lerpc(weapon_fire_recoil, GLM_VEC2_ZERO, time_delta * 6.0f, weapon_fire_recoil);

		player_dir[0] = cosf(glm_rad(player_angle[0] + weapon_fire_recoil[0])) * cosf(glm_rad(player_angle[1] + weapon_fire_recoil[1]));
		player_dir[1] = sinf(glm_rad(player_angle[1] + weapon_fire_recoil[1]));
		player_dir[2] = sinf(glm_rad(player_angle[0] + weapon_fire_recoil[0])) * cosf(glm_rad(player_angle[1] + weapon_fire_recoil[1]));
		glm_normalize(player_dir);

		ncx_renderer_mouse_pos_set(&renderer, (vec2){renderer.is_fullscreen ? renderer.monitor_size[0] / 2.0f : WINDOW_WIDTH / 2.0f, renderer.is_fullscreen ? renderer.monitor_size[1] / 2.0f : WINDOW_HEIGHT / 2.0f});

		{ /* floor collision */
			const uint8_t jump_press_old = jump_press;
			vec3 *floor_vert_positions = malloc(ncx_model_room->meshes->index_count * sizeof(vec3));
			vec3 *floor_vert_normals = malloc(ncx_model_room->meshes->index_count * sizeof(vec3));

			jump_press = ncx_renderer_key_get_press(renderer, GLFW_KEY_SPACE);
			if(jump_press && jump_press != jump_press_old && !player_jumping) {
				player_vertical_velocity -= 0.02f;
				player_jumping = 1;
			}

			for(uint32_t i = 0; i < ncx_model_room->meshes->index_count; i++) {
				glm_vec3_copy(ncx_model_room->meshes->vertices[ncx_model_room->meshes->indices[i]].pos, floor_vert_positions[i]);
				glm_vec3_copy(ncx_model_room->meshes->vertices[ncx_model_room->meshes->indices[i]].normal, floor_vert_normals[i]);
			}

			player_vertical_velocity += 8.4f * time_delta * time_delta;
			player_pos_b[1] -= player_vertical_velocity;
			player_pos_a[1] = player_pos_b[1];
			for(uint16_t i = 0; i < ncx_model_room->meshes->index_count / 3; i++) {
				vec3 p0, p1;
				vec3 face_normal;
				float distance = 0.0f;

				glm_vec3_sub(floor_vert_positions[1 + (i * 3)], floor_vert_positions[0 + (i * 3)], p0);
				glm_vec3_sub(floor_vert_positions[2 + (i * 3)], floor_vert_positions[0 + (i * 3)], p1);
				glm_cross(p0, p1, face_normal);
				glm_normalize(face_normal);

				if(glm_ray_triangle(player_pos_b, (vec3){0.0f, -1.0f, 0.0f}, floor_vert_positions[0 + (i * 3)], floor_vert_positions[1 + (i * 3)], floor_vert_positions[2 + (i * 3)], &distance)) {
					if(distance < 1.0f) {
						const float player_vertical_velocity_old = player_vertical_velocity;
						player_pos_b[1] += (1.0f - distance);
						player_vertical_velocity = 0.0f;
						if(fabsf(player_vertical_velocity - player_vertical_velocity_old) > EPSILON * time_delta * time_delta * 48000.0f) {
							ncx_sound_play(footstep_sounds, player_vertical_velocity_old * 35.0f, 0.5f, GLM_VEC3_ZERO, 0, 0);
							player_jumping = 0;
							// printf("%f\n", (double)player_vertical_velocity_old);
						}
					} else {
						const float dot = glm_dot(face_normal, GLM_YUP);
						if(dot == 1.0f) {
							player_headbob_intensity = ncx_clampf(player_headbob_intensity - (time_delta * 512.0f), 0.0f, 100.0f);
						} else {
							if(!player_jumping) {
								player_pos_b[1] += (1.0f - distance);
								player_vertical_velocity = 0.0f;
							} else {
								player_headbob_intensity = ncx_clampf(player_headbob_intensity - (time_delta * 512.0f), 0.0f, 100.0f);
							}
						}
					}
				}
			}

			free(floor_vert_normals);
			free(floor_vert_positions);
		}

		{ /* wall collision */
			vec3 *wall_vert_positions = malloc(ncx_model_room->meshes[1].index_count * sizeof(vec3));
			vec3 *wall_vert_normals = malloc(ncx_model_room->meshes[1].index_count * sizeof(vec3));

			for(uint32_t i = 0; i < ncx_model_room->meshes[1].index_count; i++) {
				glm_vec3_copy(ncx_model_room->meshes[1].vertices[ncx_model_room->meshes[1].indices[i]].pos, wall_vert_positions[i]);
				glm_vec3_copy(ncx_model_room->meshes[1].vertices[ncx_model_room->meshes[1].indices[i]].normal, wall_vert_normals[i]);
			}

			for(uint16_t i = 0; i < ncx_model_room->meshes[1].index_count / 3; i++) {
				vec3 p0, p1;
				vec3 face_normal;
				vec3 wall_raycast_dir;
				float distance = 0.0f;

				glm_vec3_sub(wall_vert_positions[1 + (i * 3)], wall_vert_positions[0 + (i * 3)], p0);
				glm_vec3_sub(wall_vert_positions[2 + (i * 3)], wall_vert_positions[0 + (i * 3)], p1);
				glm_cross(p0, p1, face_normal);
				glm_normalize(face_normal);

				glm_vec3_negate_to(face_normal, wall_raycast_dir);
				if(glm_ray_triangle(player_pos_b, wall_raycast_dir, wall_vert_positions[0 + (i * 3)], wall_vert_positions[1 + (i * 3)], wall_vert_positions[2 + (i * 3)], &distance)) {
					if(distance < 0.3f) {
						vec3 push_vector;
						glm_vec3_scale(face_normal, 0.3f - distance, push_vector);
						glm_vec3_add(player_pos_b, push_vector, player_pos_b);
						player_headbob_intensity = ncx_lerpc(player_headbob_intensity, 0.0f, time_delta * PLAYER_HEADBOB_LERP_SPEED);
					}
				}
			}
		}

		ncx_vec3_lerpc(player_pos_a, player_pos_b, (float)time_delta * PLAYER_LERP_SPEED, player_pos_a);
		player_pos_a[1] = player_pos_b[1];

		{ /* handing player movement */
			float player_speed = PLAYER_MOVE_SPEED;
			vec3 player_move;

			uint8_t keys[5] = {
				(uint8_t)ncx_renderer_key_get_press(renderer, GLFW_KEY_A),
				(uint8_t)ncx_renderer_key_get_press(renderer, GLFW_KEY_D),
				(uint8_t)ncx_renderer_key_get_press(renderer, GLFW_KEY_W),
				(uint8_t)ncx_renderer_key_get_press(renderer, GLFW_KEY_S),
				0
			};

			right_click_old = right_click;
			right_click = ncx_renderer_mouse_button_get(renderer, GLFW_MOUSE_BUTTON_RIGHT);
			keys[4] = ncx_renderer_key_get_press(renderer, GLFW_KEY_LEFT_SHIFT) * (bong_smoke_timer == 5.0f) * !(weapon_selected & WEAPON_PISTOL && right_click) * (pistol_reload_timer < EPSILON);

			player_speed += keys[4] * PLAYER_MOVE_SPEED;

			glm_vec3_copy(GLM_VEC3_ZERO, player_move);
			glm_cross(player_dir, GLM_YUP, player_right);
			glm_normalize(player_right);

			if(keys[0]) {
				glm_vec3_sub(player_move, player_right, player_move);
			}

			if(keys[1]) {
				glm_vec3_add(player_move, player_right, player_move);
			}

			if(keys[2]) {
				glm_vec3_add(player_move, player_dir, player_move);
			}

			if(keys[3]) {
				glm_vec3_sub(player_move, player_dir, player_move);
			}

			player_move[1] = 0.0f;
			glm_vec3_normalize(player_move);
			player_headbob_intensity = ncx_lerpc(player_headbob_intensity, glm_vec3_norm(player_move) * (right_click ? (12.0f * (weapon_selected & WEAPON_PISTOL * 2)) : 92.0f), time_delta * PLAYER_HEADBOB_LERP_SPEED);
			glm_vec3_scale(player_move, time_delta, player_move);
			glm_vec3_scale(player_move, player_speed, player_move);
			glm_vec3_add(player_pos_b, player_move, player_pos_b);

			{
				const float sin_player_headbob_timer_old = sinf(player_headbob_timer * GLM_PIf * 2);
				vec3 player_move_unscaled;
				float player_move_mag;
				float sin_player_headbob_timer_new;

				glm_vec3_scale(player_move, 1 / time_delta, player_move_unscaled);
				player_move_mag = glm_vec3_norm2(player_move_unscaled);

				player_headbob_timer += (player_headbob_intensity * time_delta * ((float)(keys[4] + 1) * 0.0096f));
				sin_player_headbob_timer_new = sinf(player_headbob_timer * GLM_PIf * 2);
				if(signbit(sin_player_headbob_timer_old) != signbit(sin_player_headbob_timer_new)) {
					const float footstep_pitch = 1.0f - ((float)(rand() % 256) / 1024.0f);
					const uint8_t footstep_index = (uint8_t)(rand() % 9);
					footstep_volume = (sqrtf(player_move_mag) * 0.06f);
					footstep_sounds.delay_timer = 0.0f;
					ncx_sound_play(footstep_sounds, footstep_volume, footstep_pitch, GLM_VEC3_ZERO, 0, footstep_index);
				}
			}
		}

		{
			vec3 player_look_pos_bobbed;

			player_headbob[0] = cosf(player_headbob_timer * GLM_PIf * 2) * PLAYER_HEADBOB_INTENSITY;
			player_headbob[1] = sinf(player_headbob_timer * GLM_PIf * 4) * PLAYER_HEADBOB_INTENSITY * 0.66f;
			player_headbob[2] = cosf(player_headbob_timer * GLM_PIf * 2) * PLAYER_HEADBOB_INTENSITY;

			player_headbob[0] *= -sinf(glm_rad(player_angle[0]));
			player_headbob[2] *= cosf(glm_rad(player_angle[0]));
			glm_vec3_scale(player_headbob, player_headbob_intensity * 0.0096f, player_headbob);

			glm_vec3_add(player_pos_a, player_dir, player_look_pos);
			glm_vec3_add(player_pos_a, player_headbob, player_pos_a_bobbed);
			glm_vec3_add(player_look_pos, player_headbob, player_look_pos_bobbed);
			glm_lookat(player_pos_a_bobbed, player_look_pos_bobbed, GLM_YUP, matrix_view);
		}

		/* drawing */
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL - draw_mode);

		// ncx_renderer_bind_fbo(
		glBindFramebuffer(GL_FRAMEBUFFER, renderer.sbos[0].fbo);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{
			const float calculated_aspect = (((float)sin(time_now * GLM_PI) + 1.0f) * player_trip_intensity * 0.04f) + WINDOW_ASPECT;
			const float fov_trippy_offset = (((float)cos(time_now * GLM_PI) + 1.0f) * player_trip_intensity * 0.04f);
			player_fov = ncx_lerpc(player_fov,
					(right_click && pistol_reload_timer <= 0.0f && weapon_selected & WEAPON_PISTOL) ?
					glm_rad(48.0f)
					:
					glm_rad(68.0f) + fov_trippy_offset, time_delta * 16.0f
			);

			glm_perspective(player_fov, calculated_aspect, 0.1f, 1000.0f, matrix_projection);
		}

		ncx_model_shader_set_params(matrix_view, matrix_projection, player_pos_a, (float)time_now, player_trip_intensity);
		ncx_model_shader_set_render_layer(0);
		{ /* drawing room */
			mat4 ncx_model_room_mat;
			glm_mat4_identity(ncx_model_room_mat);
			ncx_model_shader_set_matrix_model(ncx_model_room_mat);
			ncx_model_draw(ncx_model_room[(player_trip_intensity > 0.0f)], 0);
		}

		ncx_model_shader_set_render_layer(1);
		glBindFramebuffer(GL_FRAMEBUFFER, renderer.sbos[1].fbo);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{ /* drawing weapon */
			const ncx_model_t weapon_models[2] = {ncx_model_pistol, ncx_model_bong};
			mat4 ncx_model_weapon_mat;
			vec3 weapon_pos_tar;
			vec3 weapon_bob;
			vec2 weapon_angle_tar;

			glm_vec3_copy(player_pos_a_bobbed, weapon_pos_tar);
			weapon_pos_tar[1] = player_pos_a[1] - (right_click && weapon_selected & WEAPON_PISTOL && pistol_reload_timer < EPSILON ? 0.2f : 0.26f);
			{
				switch(weapon_selected) {
					case WEAPON_PISTOL: {
						if(right_click && pistol_reload_timer <= 0.0f) {
							glm_vec3_add(weapon_pos_tar,
								(vec3){(cosf(glm_rad(player_angle[0])) * 0.2f) + (0.0f * sinf(glm_rad(-player_angle[0]))),
								0.104f,
								(sinf(glm_rad(player_angle[0])) * 0.2f) + (0.0f * cosf(glm_rad(player_angle[0])))},
								weapon_pos_tar);
						} else {
							glm_vec3_add(weapon_pos_tar,
								(vec3){(cosf(glm_rad(player_angle[0])) * 0.48f) + (0.32f * sinf(glm_rad(-player_angle[0]))),
										0.0f,
									   (sinf(glm_rad(player_angle[0])) * 0.48f) + (0.32f * cosf(glm_rad(player_angle[0])))},
								weapon_pos_tar);
						}

						weapon_angle_tar[0] = glm_rad(-player_angle[0] + 180.0f);
						weapon_angle_tar[1] = 0.0f;
						ncx_vec2_lerpc(weapon_angle, weapon_angle_tar, time_delta * 16.0f, weapon_angle);

						break;
					}

					case WEAPON_BONG: {
						if(right_click && bong_smoke_cooldown_timer == 0.0f && bong_weed_count > 0 && bong_weed_tolerance_tar < 1.0f) {
							if(right_click != right_click_old) {
								ncx_sound_play(lighter_sounds, 1.0f, 1.0f, GLM_VEC3_ZERO, 0, (uint8_t)(rand() % 10));
							}

							weapon_angle_tar[0] = glm_rad(-player_angle[0] + 180.0f);
							weapon_angle_tar[1] = glm_rad(54.0f);

							glm_vec3_add(weapon_pos_tar,
								(vec3){(cosf(glm_rad(player_angle[0])) * 0.32f),
								0.02f,
								(sinf(glm_rad(player_angle[0])) * 0.32f)}, weapon_pos_tar);

							{
								const float bong_smoke_timer_old = bong_smoke_timer;
								const float bong_pull_first_time = BONG_TIMER_MAX - (BONG_TIMER_MAX / 8);
								const float bong_pull_second_time = BONG_TIMER_MAX - (BONG_TIMER_MAX / 2);
								if(bong_smoke_timer > 0.0f) {
									player_angle[1] = ncx_lerpc(player_angle[1], -0.2f * 180.0f, time_delta * 8.0f);
									bong_smoke_timer -= time_delta;
									if((bong_smoke_timer < bong_pull_first_time) && (bong_smoke_timer_old >= bong_pull_first_time)) {
										ncx_sound_play(bong_pull_sounds, 0.2f, 1.0f, GLM_VEC3_ZERO, 0, (uint8_t)(rand() % 3));
									}

									if((bong_smoke_timer < bong_pull_second_time) && (bong_smoke_timer_old >= bong_pull_second_time)) {
										ncx_sound_play(bong_pull_sounds, 0.6f, 1.0f, GLM_VEC3_ZERO, 0, (uint8_t)(rand() % 3) + 3);
									}
								} else {
									const uint8_t tripping_music_old = tripping_music_playing;
									do {
										tripping_music_playing = (uint8_t)(rand() % 5);
									} while(tripping_music_playing == tripping_music_old);
									ncx_sound_play(tripping_music, 1.0f, 1.0f, GLM_VEC3_ZERO, 0, tripping_music_playing);
									bong_smoke_inhaled = 1;
									bong_smoke_timer = 0.0f;
								}

								/* events just after we inhale bong smoke */
								if(bong_smoke_inhaled) {
									footstep_sounds.delay_timer = 500.0f;
									pistol_fire_sound.delay_timer = 500.0f;
									pistol_reload_sound.delay_timer = 500.0f;
									pistol_click_sound.delay_timer = 500.0f;
									bong_smoke_cooldown_timer_start = (BONG_COOLDOWN_TIME - (bong_weed_tolerance_tar * BONG_COOLDOWN_TIME));
									bong_smoke_cooldown_timer = bong_smoke_cooldown_timer_start;

									bong_weed_tolerance_tar = ncx_clampf01(bong_weed_tolerance_tar + 0.64f);
									bong_weed_count--;
									bong_smoke_inhaled = 0;
								}
							}
						} else {
							glm_vec3_add(weapon_pos_tar,
								(vec3){(cosf(glm_rad(player_angle[0])) * 0.38f) + (0.28f * sinf(glm_rad(-player_angle[0]))),
										0.0f,
									   (sinf(glm_rad(player_angle[0])) * 0.38f) + (0.28f * cosf(glm_rad(player_angle[0])))},
								weapon_pos_tar);

							weapon_angle_tar[0] = glm_rad(-player_angle[0] + 180.0f);
							weapon_angle_tar[1] = 0.0f;
							bong_smoke_timer = BONG_TIMER_MAX;

							alSourceStop(bong_pull_sounds.source);
						}
						break;
					}
				}

				alSourcef(tripping_music.source, AL_GAIN, player_trip_intensity / 4);
			}

			glm_mat4_identity(ncx_model_weapon_mat);
			ncx_vec3_lerpc(weapon_pos, weapon_pos_tar, time_delta * (24.0f + ((float)((right_click * 32.0f) + ((weapon_selected & WEAPON_PISTOL) * 32.0f)))), weapon_pos);
			ncx_vec2_lerpc(weapon_angle, weapon_angle_tar, time_delta * (8.0f + ((float)(right_click && weapon_selected & WEAPON_PISTOL) * 42.0f)), weapon_angle);
			weapon_pos[1] = weapon_pos_tar[1];

			/* trippy audio effects */
			if(bong_smoke_cooldown_timer > 0.0f) {
				const float new_pitch = (sinf(bong_smoke_cooldown_timer * 25.0f * GLM_PIf) / 2) + 1.2f;
				ncx_sound_play_delay(&footstep_sounds, footstep_volume, new_pitch, GLM_VEC3_ZERO, 0, time_delta);
				ncx_sound_play_delay(&pistol_fire_sound, 0.25f, new_pitch, GLM_VEC3_ZERO, 0, time_delta);
				ncx_sound_play_delay(&pistol_click_sound, 0.25f, new_pitch, GLM_VEC3_ZERO, 0, time_delta);
				ncx_sound_play_delay(&pistol_reload_sound, 0.25f, new_pitch, GLM_VEC3_ZERO, 0, time_delta);
				alSourcef(footstep_sounds.source, AL_PITCH, new_pitch);
				alSourcef(pistol_fire_sound.source, AL_PITCH, new_pitch);
				alSourcef(pistol_click_sound.source, AL_PITCH, new_pitch);
				alSourcef(pistol_reload_sound.source, AL_PITCH, new_pitch);
			} else {
				bong_weed_tolerance_tar = ncx_clampf01(bong_weed_tolerance_tar - (time_delta / 80));
			}

			if(bong_smoke_cooldown_timer > 0.0f) {
				float t = (bong_smoke_cooldown_timer_start - bong_smoke_cooldown_timer) / 7;
				t = t * t * t;
				bong_smoke_cooldown_timer -= time_delta;
				player_trip_intensity = ncx_clampf01(t);
			} else {
				player_trip_intensity = ncx_clampf01(player_trip_intensity - (time_delta / 7));
				bong_smoke_cooldown_timer = 0.0f;
			}

			glm_translate(ncx_model_weapon_mat, weapon_pos);

			glm_vec3_copy(player_headbob, weapon_bob);
			glm_vec3_scale(weapon_bob, -0.3f, weapon_bob);
			glm_translate(ncx_model_weapon_mat, weapon_bob);

			glm_rotate(ncx_model_weapon_mat, weapon_angle[1], player_right);
			glm_rotate(ncx_model_weapon_mat, weapon_angle[0], GLM_YUP);

			{
				vec3 player_look_pos_even;
				glm_vec3_copy(player_look_pos, player_look_pos_even);
				player_look_pos_even[1] = player_pos_a[1];
				glm_lookat(player_pos_a, player_look_pos_even, GLM_YUP, matrix_view);
			}

			ncx_model_shader_set_matrix_view(matrix_view);
			ncx_model_shader_set_matrix_model(ncx_model_weapon_mat);
			switch(weapon_selected) {
				case WEAPON_PISTOL:
					ncx_model_draw(weapon_models[0], 0);
					break;

				case WEAPON_BONG:
					ncx_model_draw(weapon_models[1], 0);
					break;
			}
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		#ifdef DEBUG
		{
			int (*print_functions[3])(char *, const char *, const float *);
			print_functions[0] = &ncx_sprintfv1;
			print_functions[1] = &ncx_sprintfv2;
			print_functions[2] = &ncx_sprintfv3;

			const float time_half = (float)time_now / 2.0f;
			const float flash_timer = time_half - (float)((int32_t)time_half);

			const char *formats[] = {
				"X: %.1f, Y: %.1f, Z: %.1f",
				"Yaw: %.1f, Pitch: %.1f",
				"Trip Intensity: %.2f",
				"Trip Timer: %.2f",
				"Bong Smoke Timer: %.2f",
				"Time Passed: %.2f",
				"Framerate: %.0f",
				"Fucks I give: %.0f",
			};

			#define BUFFER_COUNT 8
			const uint8_t vector_size[BUFFER_COUNT] = { 3, 2, 1, 1, 1, 1, 1, 1 };
			const float fucks_i_give = fabsf(0.0f / 0.0f);
			const float framerate = 1.0f / time_delta;
			const float timef = (float)time_now;
			const float *values[BUFFER_COUNT] = {
				player_pos_a,
				player_angle,
				&player_trip_intensity,
				&bong_smoke_cooldown_timer,
				&bong_smoke_timer,
				&timef,
				&framerate,
				&fucks_i_give
			};

			char buffers[BUFFER_COUNT][64];

			if(flash_timer < 0.75f)
				ncx_font_draw(shagadelic_font, "DEBUG BUILD", (vec2){0.0333f, 0.9028f}, GLM_VEC3_ONE, 1.0f, WINDOW_SIZE);

			for(uint8_t i = 0; i < BUFFER_COUNT; i++) {
				print_functions[vector_size[i] - 1](buffers[i], formats[i], values[i]);
				ncx_font_draw(shagadelic_font, buffers[i], (vec2){0.0333f, 0.8528f - (0.05f * (float)i)}, GLM_VEC3_ONE, 0.8f, WINDOW_SIZE);
			}
		}
		#endif

		bong_weed_tolerance = ncx_lerpc(bong_weed_tolerance, bong_weed_tolerance_tar, time_delta * 1.8f);
		switch(weapon_selected) {
			case WEAPON_PISTOL: {
				char ammo_buffer[8];
				sprintf(ammo_buffer, "%u/%u", pistol_ammo_loaded, pistol_ammo_reserve);
				ncx_font_draw(shagadelic_font, ammo_buffer, (vec2){0.114f, 0.069f}, GLM_VEC3_ONE, 1.0f, WINDOW_SIZE);
				break;
			}

			case WEAPON_BONG: {
				char weed_buffer[16];
				if(fabsf(bong_weed_tolerance_tar - bong_weed_tolerance) < EPSILON * 4)
					bong_weed_tolerance = bong_weed_tolerance_tar;

				sprintf(weed_buffer, "Weed: %i", bong_weed_count);
				ncx_font_draw(shagadelic_font, weed_buffer, (vec2){0.1083f, 0.0592f}, GLM_VEC3_ONE, 1.0f, WINDOW_SIZE);
				sprintf(weed_buffer, "Tolerance: %i%c", (uint32_t)(bong_weed_tolerance * 100.0f), '%');
				ncx_font_draw(shagadelic_font, weed_buffer, (vec2){0.1083f, 0.1185f}, GLM_VEC3_ONE, 1.0f, WINDOW_SIZE);
				break;
			}
		}

		glm_vec2_copy((vec2){0.0333f * WINDOW_WIDTH, 0.8223f * WINDOW_HEIGHT}, ncx_ui_weapon_slot.pos);
		glm_vec2_copy((vec2){0.0667f * WINDOW_WIDTH, 0.0667f * WINDOW_WIDTH}, ncx_ui_weapon_slot.size);
		ncx_ui_elements_set_flash(pistol_fire_timer * 4 * (float)(!pistol_ammo_loaded && !pistol_ammo_reserve));
		ncx_ui_element_draw(ncx_ui_weapon_slot, 0);

		glm_vec2_copy((vec2){0.0416f * WINDOW_WIDTH, 0.8369f * WINDOW_HEIGHT}, ncx_ui_weapon_slot.pos);
		glm_vec2_copy((vec2){0.05f * WINDOW_WIDTH, 0.05f * WINDOW_WIDTH}, ncx_ui_weapon_slot.size);
		ncx_ui_element_draw(ncx_ui_weapon_slot, weapon_selected);
		ncx_ui_elements_set_flash(0.0f);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ncx_renderer_draw(renderer, trippy_texture, (float)time_now, player_trip_intensity);
		ncx_renderer_swap_buffers(renderer);
		// printf("%f\n", 1.0 / (double)time_delta);
	}

	ncx_model_destroy(&ncx_model_bong);
	ncx_model_destroy(&ncx_model_pistol);
	ncx_model_destroy(ncx_model_room + 1);
	ncx_model_destroy(ncx_model_room);
	ncx_model_shader_destroy();

	glDeleteTextures(1, &trippy_texture);
	glDeleteTextures(2, bong_textures);
	glDeleteTextures(2, pistol_textures);
	glDeleteTextures(6, room_textures);

	glDeleteTextures(3, ncx_ui_weapon_slot_textures);

	ncx_ui_element_destroy(&ncx_ui_weapon_slot);
	ncx_ui_elements_terminate();

	ncx_font_destroy(&shagadelic_font);
	ncx_font_shader_destroy();

	ncx_sound_destroy(&tripping_music);
	
	ncx_sound_destroy(&pistol_reload_sound);
	ncx_sound_destroy(&pistol_click_sound);
	ncx_sound_destroy(&pistol_fire_sound);

	ncx_sound_destroy(&bong_pull_sounds);
	ncx_sound_destroy(&lighter_sounds);
	ncx_sound_destroy(&footstep_sounds);

	ncx_renderer_destroy(&renderer);

	return 0;
}

#include "narcotix/cglm/mat4.h"
#include "narcotix/model.h"
#include "narcotix/renderer.h"
#include "narcotix/font.h"
#include "narcotix/texture.h"
#include "narcotix/sound_engine.h"
#include "narcotix/sound.h"
#include "narcotix/helpers.h"

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080
#define WINDOW_SIZE ((vec2){WINDOW_WIDTH, WINDOW_HEIGHT})
#define WINDOW_ASPECT ((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT)

int main() {
	NCXRenderer renderer;
	NCXSoundEngine sound_engine;

	NCXTexture trippy_texture;

	NCXFont trippy_font;
	NCXFont normal_font;
	NCXLightPoint lights[2];

	NCXMaterial pistol_materials[4];
	NCXModel pistol_model;

	NCXMaterial bong_materials[3];
	NCXModel bong_model;

	NCXSound test_sound;

	mat4 projection;
	mat4 view;
	mat4 model_matrix;

	renderer = ncx_renderer_create(WINDOW_WIDTH, WINDOW_HEIGHT, 2, "Narcotix Test", 1);
	sound_engine = ncx_sound_engine_create();
	test_sound = ncx_sound_create("res/audio/test.wav", 1, 0);

	ncx_font_shader_create("res/shaders/font_vert.glsl", "res/shaders/font_frag.glsl");
	trippy_font = ncx_font_create("res/fonts/shagadelic.ttf");
	normal_font = ncx_font_create("res/fonts/jetbrainsmono-bold.ttf");

	trippy_texture = ncx_texture_create("res/textures/trippy-overlay-texture.png", GL_MIRRORED_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1);

	lights[0] = ncx_light_point_create(GLM_VEC3_ONE, GLM_VEC3(0.1f, 0.1f, 0.1f), GLM_VEC3(0.5f, 0.5f, 0.5f), GLM_VEC3_ONE, 1.0f, 0.09f, 0.032f);
	lights[1] = ncx_light_point_create(GLM_VEC3(-1.0f, 1.0f, 1.0f), GLM_VEC3(0.1f, 0.1f, 0.1f), GLM_VEC3(0.5f, 0.5f, 0.5f), GLM_VEC3_ONE, 1.0f, 0.09f, 0.032f);
	ncx_model_shader_create(lights, 1);

	{ /* load models */
		const char *paths[2] = {
			"res/models/weapons/pistol/pistol_diffuse.png",
			"res/models/weapons/pistol/pistol_specular.png"
		};

		/* loading the two main textures */
		pistol_materials[0] = ncx_material_create(paths, (float)(1 << 3));

		/* assinging pistol textures */
		for(uint8_t i = 0; i < 4; i++) {
			pistol_materials[i] = pistol_materials[0];
		}
		pistol_model = ncx_model_create("res/models/weapons/pistol/pistol.glb", pistol_materials);

		/* assinging bong textures */
		for(uint8_t i = 0; i < 3; i++) {
			bong_materials[i] = pistol_materials[0];
		}
		bong_model = ncx_model_create("res/models/weapons/bong/bong.glb", bong_materials);
	}

	glm_perspective(glm_rad(45.0f), WINDOW_ASPECT, 0.1f, 32.0f, projection);
	glm_mat4_identity(view);
	glm_translate(view, (vec3){0.0f, 0.0f, -1.0f});
	ncx_sound_play(test_sound, 0.2f, 1.0f, GLM_VEC3_ZERO, 1, 0);
	while(ncx_renderer_running_get(renderer)) {
		const float time_now = (float)glfwGetTime();
		const float trip_intensity = (sinf(time_now) + 1.0f) / 2.0f;

		/* checking for quit */
		if(ncx_renderer_key_get_press(renderer, GLFW_KEY_ESCAPE)) {
			ncx_renderer_running_set(renderer, 0);
			break;
		}

		/* drawing gun */
		glm_mat4_identity(model_matrix);
		glm_translate(model_matrix, (vec3){-0.2f, 0.0f, 0.0f});
		glm_rotate(model_matrix, time_now, GLM_YUP);

		ncx_renderer_bind_sbo(renderer, 0);
		ncx_renderer_clear_color(0.16f, 0.32f, 0.42f, 1.0f);
		ncx_renderer_clear_depth();

		ncx_model_shader_set_render_layer(0);
		ncx_model_shader_set_params(view, projection, GLM_VEC3_ZERO, time_now, trip_intensity);
		ncx_model_shader_set_matrix_model(model_matrix);
		ncx_model_draw(pistol_model, 0);

		/* drawing bong */
		glm_mat4_identity(model_matrix);
		glm_scale(model_matrix, (vec3){1.32f, 1.32f, 1.32f});
		glm_translate(model_matrix, (vec3){0.2f, -0.14f, 0.0f});
		glm_rotate(model_matrix, time_now, GLM_YUP);
		ncx_model_shader_set_matrix_model(model_matrix);
		ncx_model_draw(bong_model, 0);

		ncx_renderer_bind_sbo(renderer, 1);
		ncx_model_shader_set_render_layer(1);
		ncx_renderer_clear_color(0.0f, 0.0f, 0.0f, 0.0f);
		ncx_font_draw(trippy_font, "Narcotix Engine Test", (vec2){0.02f, 0.92f}, GLM_VEC3_ONE, 0.8f, WINDOW_SIZE);
		ncx_font_draw(normal_font, "Music: 'Sandworms - Andy Caldwell VS. Darkhorse' from Mushroom Jazz 2", (vec2){0.02f, 0.04f}, GLM_VEC3_ONE, 0.5f, WINDOW_SIZE);

		ncx_renderer_unbind_sbo();
		ncx_renderer_display(renderer, trippy_texture, time_now, trip_intensity);
		ncx_renderer_poll(renderer);
	}


	ncx_sound_destroy(&test_sound);
	ncx_sound_engine_destroy(sound_engine);
	
	ncx_model_destroy(&bong_model);
	ncx_model_destroy(&pistol_model);
	ncx_model_shader_destroy();

	ncx_textures_destroy(&trippy_texture, 1);

	ncx_materials_destroy(bong_materials, 3);
	ncx_materials_destroy(pistol_materials, 4);

	ncx_font_destroy(&normal_font);
	ncx_font_destroy(&trippy_font);
	ncx_font_shader_destroy();

	ncx_renderer_destroy(&renderer);
	return 0;
}

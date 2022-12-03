#include "narcotix/context.h"
#include "narcotix/sound_engine.h"
#include "narcotix/sound.h"
#include "narcotix/font.h"
#include "narcotix/light_point.h"
#include "narcotix/helpers.h"
#include "narcotix/model.h"

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080
#define WINDOW_SIZE ((vec2){WINDOW_WIDTH, WINDOW_HEIGHT})
#define WINDOW_ASPECT ((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT)

int main() {
	NCXContext context =
		ncx_context_create(WINDOW_WIDTH, WINDOW_HEIGHT, 2, "Narcotix Test", 1);
	NCXSoundEngine sound_engine = ncx_sound_engine_create();
	NCXSound test_sound = ncx_sound_create("res/audio/test.wav", 1, 0);

	NCXShader font_shader =
		ncx_font_shader_create("res/shaders/builtin/font_vert.glsl",
			"res/shaders/builtin/font_frag.glsl");
	NCXFont trippy_font = ncx_font_create("res/fonts/shagadelic.ttf");
	NCXFont normal_font = ncx_font_create("res/fonts/jetbrainsmono-bold.ttf");

	NCXTexture trippy_texture =
		ncx_texture_create("res/textures/trippy-overlay-texture.png",
				GL_MIRRORED_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1);

	NCXLightPoint lights[2] = {
		ncx_light_point_create(GLM_VEC3( 1.0f, 0.0f, 1.0f),
				GLM_VEC3(0.1f, 0.1f, 0.1f), GLM_VEC3(1.0f, 0.0f, 0.0f),
				GLM_VEC3(1.0f, 0.0f, 0.0f), 1.0f, 0.09f, 0.032f),

		ncx_light_point_create(GLM_VEC3(-1.0f, 0.0f, 1.0f),
				GLM_VEC3(0.1f, 0.1f, 0.1f), GLM_VEC3(0.0f, 1.0f, 1.0f),
				GLM_VEC3(0.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f),
	};

	/* load models */
	NCXShader model_shader = ncx_model_shader_create(lights, 2);

	const char *pistol_tex_paths[M_COUNT] = {
		"res/models/weapons/pistol/pistol_diffuse.png",
		"res/models/weapons/pistol/pistol_specular.png",
		"res/models/weapons/pistol/pistol_normal.png"
	};

	NCXMaterial pistol_materials[4] = {
		ncx_material_create(pistol_tex_paths, 8),
	};

	for(uint8_t i = 0; i < 4; i++) {
		pistol_materials[i] = pistol_materials[0];
	}

	NCXModel pistol_model =
		ncx_model_create("res/models/weapons/pistol/pistol.glb",
				pistol_materials, 1);
		ncx_model_animation_set(&pistol_model, 1);

	NCXMaterial bong_materials[3] = {
		pistol_materials[0],
		pistol_materials[0],
		pistol_materials[0],
	};
	NCXModel bong_model = ncx_model_create("res/models/weapons/bong/bong.glb",
			bong_materials, 0);

	const char *brick_tex_paths[M_COUNT] = {
		"res/textures/bricks_diffuse.png",
		"res/textures/bricks_specular.png",
		"res/textures/bricks_normal.png",
	};

	NCXMaterial brick_material = ncx_material_create(brick_tex_paths, 16);
	NCXModel brick_model = ncx_model_create("res/models/plane.glb",
			&brick_material, 0);

	mat4 projection;
	glm_perspective(glm_rad(45.0f), WINDOW_ASPECT, 0.1f, 32.0f, projection);
	
	mat4 view;
	glm_mat4_identity(view);
	glm_translate(view, GLM_VEC3(0.0f, 0.0f, -1.0f));
	
	ncx_context_time_delta_init();
	while(ncx_context_window_is_running(context)) {

		/* checking for quit */
		if(ncx_context_key_get(context, GLFW_KEY_ESCAPE)) {
			ncx_context_window_close(context);
			break;
		}

		/* drawing gun */
		ncx_context_render_buffer_bind(context, 0);
		ncx_context_clear_color(0.0f, 0.055f, 0.122f, 1.0f);
		ncx_context_clear_depth();

		ncx_shader_use(model_shader);
		ncx_shader_uniform_int(model_shader, "render_layer", 0);
		ncx_shader_uniform_mat4(model_shader, "projection", projection);
		ncx_shader_uniform_mat4(model_shader, "view", view);
		ncx_shader_uniform_vec3(model_shader, "view_pos", GLM_VEC3_ZERO);

		const float trip_intensity = 0.0f;
		ncx_shader_uniform_float(model_shader, "trip_intensity",
				trip_intensity);

		const float time_now = ncx_context_time_get();
		ncx_shader_uniform_float(model_shader, "time", time_now);

		const float time_delta = ncx_context_time_delta_get();

		/* drawing pistol */
		mat4 model_mat;
		glm_mat4_identity(model_mat);
		glm_translate(model_mat, GLM_VEC3(-0.2f, 0.0f, 0.0f));
		ncx_model_animation_update(&pistol_model, time_delta, 1);
		ncx_model_draw(pistol_model, model_shader, model_mat);

		/* drawing bong */
		glm_mat4_identity(model_mat);
		glm_scale(model_mat, GLM_VEC3(1.32f, 1.32f, 1.32f));
		glm_translate(model_mat, GLM_VEC3(0.2f, -0.14f, 0.0f));
		glm_rotate(model_mat, time_now, GLM_YUP);
		ncx_meshes_draw(bong_model.meshes, bong_model.mesh_count,
				model_shader, model_mat);

		/* drawing bricks */
		glm_mat4_identity(model_mat);
		glm_translate(model_mat, GLM_VEC3(sinf(time_now), 0.0f, -1.5f));
		glm_rotate(model_mat, sinf(time_now), GLM_YUP);
		ncx_meshes_draw(brick_model.meshes, 1, model_shader, model_mat);

		ncx_context_render_buffer_bind(context, 1);
		ncx_context_clear_color(0.0f, 0.0f, 0.0f, 0.0f);

		/* drawing text */
		ncx_font_draw(trippy_font, "Narcotix Engine Test",
				GLM_VEC2(0.02f, 0.92f), GLM_VEC3_ONE, 0.8f, WINDOW_SIZE,
				font_shader);
		ncx_font_draw(normal_font,
				"Music: 'Sandworms - Andy Caldwell VS. Darkhorse'"
				" from Mushroom Jazz 2", GLM_VEC2(0.02f, 0.04f),
				GLM_VEC3_ONE, 0.5f, WINDOW_SIZE,
				font_shader);

		ncx_context_render_buffer_unbind();
		ncx_context_buffer_display(context, 0, time_now, trip_intensity);
		ncx_context_buffer_swap(context);
	}

	ncx_sound_destroy(&test_sound);
	ncx_sound_engine_destroy(sound_engine);
	
	ncx_model_destroy(&bong_model);
	ncx_model_destroy(&pistol_model);

	ncx_textures_destroy(&trippy_texture, 1);

	ncx_materials_destroy(bong_materials, 3);
	ncx_materials_destroy(pistol_materials, 4);

	ncx_font_destroy(&normal_font);
	ncx_font_destroy(&trippy_font);
	ncx_shader_destroy(font_shader);

	ncx_shader_destroy(model_shader);

	ncx_context_destroy(&context);

	return 0;
}

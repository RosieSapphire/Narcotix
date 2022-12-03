#include "narcotix/context.h"
#include "narcotix/sound_engine.h"
#include "narcotix/sound.h"
#include "narcotix/font.h"
#include "narcotix/light_point.h"
#include "narcotix/helpers.h"
#include "narcotix/model.h"
#include "narcotix/ui.h"
#include "narcotix/material.h"
#include "test_paths.h"

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080
#define WINDOW_NAME "NARCOTIX ENGINE TEST"
#define WINDOW_SIZE ((vec2){WINDOW_WIDTH, WINDOW_HEIGHT})
#define WINDOW_ASPECT ((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT)

int main() {
	ncx_context_t context =
		ncx_context_create(WINDOW_WIDTH, WINDOW_HEIGHT, 2, WINDOW_NAME, 1);

	ncx_sound_engine_init();
	ncx_sound_t test_sound = ncx_sound_create("res/audio/test.wav", 1, 0);

	ncx_shader_t font_shader =
		ncx_font_shader_create(font_shader_vert_path, font_shader_frag_path);

	ncx_font_t trippy_font = ncx_font_create(trippy_font_path);
	ncx_font_t normal_font = ncx_font_create(normal_font_path);

	ncx_texture_t trippy_texture = ncx_texture_create(trippy_tex_path,
			GL_MIRRORED_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1);

	ncx_light_point_t lights[2] = {
		ncx_light_point_create(GLM_VEC3(1.0f, 0.0f, 1.0f),
				GLM_VEC3(0.1f, 0.1f, 0.1f), GLM_VEC3(1.0f, 0.0f, 0.0f),
				GLM_VEC3(1.0f, 0.0f, 0.0f), 1.0f, 0.09f, 0.032f),

		ncx_light_point_create(GLM_VEC3(-1.0f, 0.0f, 1.0f),
				GLM_VEC3(0.1f, 0.1f, 0.1f), GLM_VEC3(0.0f, 1.0f, 1.0f),
				GLM_VEC3(0.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f),
	};

	/* load models */
	ncx_shader_t model_shader = ncx_model_shader_create(lights, 2);

	const ncx_material_data_t pistol_tex_data = {
		.diffuse_path = "res/models/weapons/pistol/pistol_diffuse.png",
		.specular_path = "res/models/weapons/pistol/pistol_specular.png",
		.normal_path = "res/models/weapons/pistol/pistol_normal.png",
		.shininess = 8,
	};

	ncx_material_t pistol_materials[4] = {
		ncx_material_create(pistol_tex_data),
	};

	for(uint8_t i = 1; i < 4; i++) {
		pistol_materials[i] = pistol_materials[0];
	}

	ncx_model_t pistol_model = ncx_model_create(pistol_model_path,
			pistol_materials, 1);

	ncx_model_animation_set(&pistol_model, 1);

	ncx_material_t bong_materials[3] = {
		pistol_materials[0],
		pistol_materials[0],
		pistol_materials[0],
	};

	ncx_model_t bong_model = ncx_model_create("res/models/weapons/bong/bong.glb",
			bong_materials, 0);


	const ncx_material_data_t brick_tex_data = {
		.diffuse_path = "res/textures/bricks_diffuse.png",
		.specular_path = "res/textures/bricks_specular.png",
		.normal_path = "res/textures/bricks_normal.png",
		.shininess = 16,
	};

	ncx_material_t brick_material = ncx_material_create(brick_tex_data);
	ncx_model_t brick_model =
		ncx_model_create(brick_model_path, &brick_material, 0);

	ncx_texture_t ui_test_tex = ncx_texture_create(ui_test_tex_path,
			GL_CLAMP_TO_EDGE, GL_LINEAR, GL_LINEAR, 0);
	ncx_ui_element_t ui_test =
		ncx_ui_element_create(GLM_VEC3_ZERO, (vec2){256, 256}, &ui_test_tex, 1);

	mat4 projection;
	glm_perspective(glm_rad(45.0f), WINDOW_ASPECT, 0.1f, 32.0f, projection);
	
	mat4 view;
	glm_mat4_identity(view);
	glm_translate(view, GLM_VEC3(0.0f, 0.0f, -1.0f));
	
	ncx_context_time_delta_init();
	while(ncx_context_window_is_running(context)) {

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

		ncx_ui_element_draw(ui_test, 0);

		ncx_context_render_buffer_unbind();
		ncx_context_buffer_display(context, 0, time_now, trip_intensity);
		ncx_context_buffer_swap(context);
	}

	ncx_sound_destroy(&test_sound);
	ncx_sound_engine_terminate();
	
	ncx_model_destroy(&bong_model);
	ncx_model_destroy(&pistol_model);

	ncx_textures_destroy(&trippy_texture, 1);
	ncx_textures_destroy(&ui_test_tex, 1);

	ncx_materials_destroy(bong_materials, 3);
	ncx_materials_destroy(pistol_materials, 4);

	ncx_font_destroy(&normal_font);
	ncx_font_destroy(&trippy_font);
	ncx_shader_destroy(font_shader);

	ncx_shader_destroy(model_shader);

	ncx_context_destroy(&context);

	return 0;
}

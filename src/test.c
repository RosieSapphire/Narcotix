#include "narcotix/context.h"
#include "narcotix/sound_engine.h"
#include "narcotix/sound.h"
#include "narcotix/shader.h"
#include "narcotix/font.h"
#include "narcotix/light_point.h"
#include "narcotix/helpers.h"
#include "narcotix/model.h"

#define ROSE_PETAL_IMPL
#include "rose_petal.h"

#include "narcotix/glad/glad.h"
#include <GLFW/glfw3.h>

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080
#define WINDOW_SIZE ((vec2){WINDOW_WIDTH, WINDOW_HEIGHT})
#define WINDOW_ASPECT ((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT)

int main() {
	rp_memory_init();

	ncx_init(WINDOW_WIDTH, WINDOW_HEIGHT, 2, "Narcotix Test", 1);
	NCXSoundEngine sound_engine = ncx_sound_engine_create();
	NCXSound test_sound = ncx_sound_create("res/audio/test.wav", 1, 0);

	ncx_font_shader_create("res/shaders/font_vert.glsl",
			"res/shaders/font_frag.glsl");
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
	ncx_model_shader_create(lights, 2);
	const char *pistol_tex_paths[M_COUNT] = {
		"res/models/weapons/pistol/pistol_diffuse.png",
		"res/models/weapons/pistol/pistol_specular.png",
		"res/models/weapons/pistol/pistol_normal.png"
	};

	const char *brick_tex_paths[M_COUNT] = {
		"res/textures/bricks_diffuse.png",
		"res/textures/bricks_specular.png",
		"res/textures/bricks_normal.png",
	};

	/* loading the two main textures */
	NCXMaterial pistol_materials[4] = {
		ncx_material_create(pistol_tex_paths, 8),
	};

	for(uint8_t i = 0; i < 4; i++) {
		pistol_materials[i] = pistol_materials[0];
	}

	NCXModel pistol_model =
		ncx_model_create("res/models/weapons/pistol/pistol.glb",
				pistol_materials);

	/* assinging bong textures */
	NCXMaterial bong_materials[3] = {
		pistol_materials[0],
		pistol_materials[0],
		pistol_materials[0],
	};

	NCXModel bong_model = ncx_model_create("res/models/weapons/bong/bong.glb",
			bong_materials);

	NCXMaterial brick_material = ncx_material_create(brick_tex_paths, 16);
	NCXModel brick_model = ncx_model_create("res/models/plane.glb",
			&brick_material);

	mat4 projection;
	glm_perspective(glm_rad(45.0f), WINDOW_ASPECT, 0.1f, 32.0f, projection);
	
	mat4 view;
	glm_mat4_identity(view);
	glm_translate(view, GLM_VEC3(0.0f, 0.0f, -1.0f));
	while(ncx_window_is_running()) {
		const float time_now = ncx_time_get();
		const float trip_intensity = 0.0f;

		/* checking for quit */
		if(ncx_key_get(GLFW_KEY_ESCAPE)) {
			ncx_window_close();
			break;
		}

		/* drawing gun */
		mat4 model_mat;
		glm_mat4_identity(model_mat);
		glm_translate(model_mat, GLM_VEC3(-0.2f, 0.0f, 0.0f));
		glm_rotate(model_mat, time_now, GLM_YUP);

		ncx_render_buffer_bind(0);
		ncx_clear_color(0.0f, 0.055f, 0.122f, 1.0f);
		ncx_clear_depth();

		ncx_model_shader_set_render_layer(0);
		ncx_model_shader_set_params(view, projection, GLM_VEC3_ZERO, time_now,
				trip_intensity);
		ncx_model_shader_set_matrix_model(model_mat);
		ncx_model_draw(pistol_model, 0);

		/* drawing bong */
		glm_mat4_identity(model_mat);
		glm_scale(model_mat, GLM_VEC3(1.32f, 1.32f, 1.32f));
		glm_translate(model_mat, GLM_VEC3(0.2f, -0.14f, 0.0f));
		glm_rotate(model_mat, time_now, GLM_YUP);
		ncx_model_shader_set_matrix_model(model_mat);
		ncx_model_draw(bong_model, 0);

		/* drawing plane */
		glm_mat4_identity(model_mat);
		// glm_translate(model_mat, GLM_VEC3(0.0f, 0.0f, -1.5f));
		glm_translate(model_mat, GLM_VEC3(sinf(time_now), 0.0f, -1.5f));
		glm_rotate(model_mat, sinf(time_now), GLM_YUP);
		ncx_model_shader_set_matrix_model(model_mat);
		ncx_model_draw(brick_model, 0);

		ncx_render_buffer_bind(1);
		ncx_model_shader_set_render_layer(1);
		ncx_clear_color(0.0f, 0.0f, 0.0f, 0.0f);
		ncx_font_draw(trippy_font, "Narcotix Engine Test",
				GLM_VEC2(0.02f, 0.92f), GLM_VEC3_ONE, 0.8f, WINDOW_SIZE);
		ncx_font_draw(normal_font,
				"Music: 'Sandworms - Andy Caldwell VS. Darkhorse'"
				" from Mushroom Jazz 2", GLM_VEC2(0.02f, 0.04f),
				GLM_VEC3_ONE, 0.5f, WINDOW_SIZE);
		ncx_render_buffer_unbind();

		ncx_buffer_display(trippy_texture, time_now, trip_intensity);
		ncx_buffer_swap();
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

	ncx_terminate();

	rp_memory_print();
	rp_memory_terminate();
	return 0;
}

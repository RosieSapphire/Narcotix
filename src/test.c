#include "ncx/context.h"
#include "ncx/sound.h"
#include "ncx/font.h"
#include "ncx/light_point.h"
#include "ncx/helpers.h"
#include "ncx/model.h"
#include "ncx/ui.h"
#include "ncx/mat4.h"
#include "ncx/tex.h"

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1080
#define WINDOW_NAME "NARCOTIX ENGINE TEST"
#define WINDOW_SIZE ncx_vec2(WINDOW_WIDTH, WINDOW_HEIGHT)
#define WINDOW_ASPECT ((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT)

int main(void)
{
	ncx_init(WINDOW_WIDTH, WINDOW_HEIGHT, 2, WINDOW_NAME, 1);

	ncx_sound_init();
	struct ncx_sound test_sound =
		ncx_sound_create("res/sfx/test.wav", 1, 0);

	ncx_shader_t font_shader =
		ncx_font_shader_create("res/shdr/internal/font_vert.glsl",
					"res/shdr/internal/font_frag.glsl");

	struct ncx_font trippy_font =
		ncx_font_create("res/font/shagadelic.ttf");

	struct ncx_font norm_font =
		ncx_font_create("res/font/jetbrainsmono-bold.ttf");

	ncx_tex_t trippy_tex =
		ncx_tex_create("res/tex/trippy.png", GL_MIRRORED_REPEAT,
				GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1);

	struct ncx_light_point lights[2] = {
		ncx_light_point_create(
				ncx_vec3(1.0f, 0.0f, 1.0f),
				ncx_vec3(0.1f, 0.1f, 0.1f),
				ncx_vec3(1.0f, 0.0f, 0.0f),
				ncx_vec3(1.0f, 0.0f, 0.0f),
				1.0f, 0.09f, 0.032f),

		ncx_light_point_create(
				ncx_vec3(-1.0f, 0.0f, 1.0f),
				ncx_vec3(0.1f, 0.1f, 0.1f),
				ncx_vec3(0.0f, 1.0f, 1.0f),
				ncx_vec3(0.0f, 1.0f, 1.0f),
				1.0f, 0.09f, 0.032f),
	};

	/* load models */
	ncx_shader_t model_shader = ncx_model_shader_create(lights, 2);

	const struct ncx_material_data pistol_tex_data = {
		.diff_path = "res/mdl/wep/pist/pist_diff.png",
		.spec_path = "res/mdl/wep/pist/pist_spec.png",
		.norm_path = "res/mdl/wep/pist/pist_norm.png",
		.shininess = 8,
	};

	struct ncx_material pistol_materials[4] = {
		ncx_material_create(pistol_tex_data),
	};

	for(uint8_t i = 1; i < 4; i++) {
		pistol_materials[i] = pistol_materials[0];
	}

	struct ncx_model pistol_model =
		ncx_model_create("res/mdl/wep/pist/pist.glb",
				pistol_materials, 1);

	ncx_model_animation_set(&pistol_model, 1);

	struct ncx_material bong_materials[3] = {
		pistol_materials[0],
		pistol_materials[0],
		pistol_materials[0],
	};

	struct ncx_model bong_model =
		ncx_model_create("res/mdl/wep/bong/bong.glb",
				bong_materials, 0);

	const struct ncx_material_data plane_tex_data = {
		.diff_path = "res/tex/brick_diff.png",
		.spec_path = "res/tex/brick_spec.png",
		.norm_path = "res/tex/brick_norm.png",
		.shininess = 16,
	};

	struct ncx_material plane_material =
		ncx_material_create(plane_tex_data);

	struct ncx_model plane_model =
		ncx_model_create("res/mdl/plane.glb", &plane_material, 0);

	struct ncx_mat4 proj = ncx_mat4_persp(45.0f, WINDOW_ASPECT, 0.1f, 4.0f);
	struct ncx_mat4 view = ncx_mat4_trans(NCX_MAT4_ID, ncx_vec3(0, 0, -1));
	
	ncx_sound_play(test_sound, 0.5f, 1.0f, NCX_VEC3_0, 1, 0);

	ncx_time_delta_init();
	while(ncx_window_is_running()) {

		/* drawing gun */
		ncx_render_buffer_bind(0);
		ncx_clear_color(0.0f, 0.055f, 0.122f, 1.0f);
		ncx_clear_depth();

		ncx_shader_use(model_shader);
		ncx_shader_uniform_int(model_shader, "render_layer", 0);
		ncx_shader_uniform_mat4(model_shader, "proj", proj);
		ncx_shader_uniform_mat4(model_shader, "view", view);
		ncx_shader_uniform_vec3(model_shader, "view_pos", NCX_VEC3_0);

		const float trip_intensity = 0.0f;
		ncx_shader_uniform_float(model_shader, "trip_intensity",
				trip_intensity);

		const float time_now = ncx_time();
		ncx_shader_uniform_float(model_shader, "time", time_now);

		const float time_delta = ncx_time_delta();

		/* drawing pistol */
		struct ncx_mat4 model_mat =
			ncx_mat4_trans(NCX_MAT4_ID,
					ncx_vec3(-0.2f, 0.0f, 0.0f));

		ncx_model_animation_update(&pistol_model, time_delta, 1);
		ncx_model_draw(pistol_model, model_shader, model_mat);

		/* drawing bong */
		model_mat = NCX_MAT4_ID;

		model_mat =
			ncx_mat4_trans(model_mat, ncx_vec3(0.2f, -0.14f, 0.0f));

		model_mat = ncx_mat4_scale_uni(model_mat, 1.32f);
		model_mat = ncx_mat4_rotate(model_mat, NCX_VEC3_Y,
				time_now * 4);

		ncx_meshes_draw(bong_model.meshes, bong_model.mesh_count,
				model_shader, model_mat);

		/* drawing brick */
		model_mat = NCX_MAT4_ID;
		model_mat = ncx_mat4_trans(model_mat,
				ncx_vec3(sinf(time_now), 0.0f, -1.5f));

		model_mat = ncx_mat4_rotate(model_mat, NCX_VEC3_Y,
				sinf(time_now));

		ncx_meshes_draw(plane_model.meshes, 1, model_shader, model_mat);

		ncx_render_buffer_bind(1);
		ncx_clear_color(0.0f, 0.0f, 0.0f, 0.0f);

		/* drawing text */
		ncx_font_draw(trippy_font, "Narcotix Engine Test",
				ncx_vec2(0.02f, 0.92f), NCX_VEC3_1,
				0.8f, WINDOW_SIZE, font_shader);

		ncx_font_draw(norm_font,
				"Music: 'Sandworms - Andy Caldwell VS. "
				"Darkhorse' from Mushroom Jazz 2",
				ncx_vec2(0.02f, 0.04f),
				NCX_VEC3_1, 0.5f, WINDOW_SIZE, font_shader);

		ncx_render_buffer_unbind();
		ncx_buffer_display(0, time_now, trip_intensity);
		ncx_buffer_swap();
	}

	ncx_sound_destroy(&test_sound);
	ncx_sound_terminate();
	
	ncx_model_destroy(&bong_model);
	ncx_model_destroy(&pistol_model);

	ncx_tex_destroy(&trippy_tex, 1);

	ncx_materials_destroy(bong_materials, 3);
	ncx_materials_destroy(pistol_materials, 4);

	ncx_font_destroy(&norm_font);
	ncx_font_destroy(&trippy_font);
	ncx_shader_destroy(font_shader);

	ncx_shader_destroy(model_shader);

	ncx_terminate();

	return 0;
}

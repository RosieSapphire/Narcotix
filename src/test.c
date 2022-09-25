#include "narcotix/cglm/mat4.h"
#include "narcotix/model.h"
#include "narcotix/renderer.h"
#include "narcotix/font.h"
#include "narcotix/texture.h"

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT  720
#define WINDOW_SIZE ((vec2){WINDOW_WIDTH, WINDOW_HEIGHT})
#define WINDOW_ASPECT ((float)WINDOW_WIDTH / (float)WINDOW_HEIGHT)

int main() {
	ncx_renderer_t renderer;

	ncx_font_t font;
	NCXLightPoint lights[2];

	NCXMaterial pistol_materials[4];
	ncx_model_t pistol_model;

	NCXMaterial bong_materials[3];
	ncx_model_t bong_model;

	mat4 projection;
	mat4 view;
	mat4 model_matrix;

	ncx_renderer_create(&renderer, WINDOW_WIDTH, WINDOW_HEIGHT, 2, "Narcotix Test");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	ncx_font_shader_create("res/shaders/font_vert.glsl", "res/shaders/font_frag.glsl");
	font = ncx_font_create("res/fonts/shagadelic.ttf");

	lights[0] = ncx_light_point_create((vec3){1.0f, 1.0f, 1.0f}, (vec3){0.1f, 0.1f, 0.1f}, (vec3){0.5f, 0.5f, 0.5f}, GLM_VEC3_ONE, 1.0f, 0.09f, 0.032f);
	lights[1] = ncx_light_point_create((vec3){-1.0f, 1.0f, 1.0f}, (vec3){0.1f, 0.1f, 0.1f}, (vec3){0.5f, 0.5f, 0.5f}, GLM_VEC3_ONE, 1.0f, 0.09f, 0.032f);
	ncx_model_shader_create(lights, 2);
	{
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
	while(ncx_renderer_running_get(renderer)) {
		const float time_now = (float)glfwGetTime();

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
		ncx_model_shader_set_render_layer(0);
		ncx_renderer_clear_color(0.16f, 0.32f, 0.42f, 1.0f);
		ncx_renderer_clear_depth();
		ncx_model_draw(pistol_model, 0, (const float *)projection, (const float *)view, (const float *)model_matrix);

		/* drawing bong */
		glm_mat4_identity(model_matrix);
		glm_scale(model_matrix, (vec3){1.32f, 1.32f, 1.32f});
		glm_translate(model_matrix, (vec3){0.2f, -0.14f, 0.0f});
		glm_rotate(model_matrix, time_now, GLM_YUP);
		ncx_model_draw(bong_model, 0, (const float *)projection, (const float *)view, (const float *)model_matrix);

		ncx_renderer_bind_sbo(renderer, 1);
		ncx_model_shader_set_render_layer(1);
		ncx_renderer_clear_color(0.0f, 0.0f, 0.0f, 0.0f);
		ncx_font_draw(font, "Narcotix Engine Test", (vec2){64.0f / WINDOW_WIDTH, 64.0f / WINDOW_HEIGHT}, GLM_VEC3_ONE, 1.0f, WINDOW_SIZE);

		ncx_renderer_unbind_sbo();
		ncx_renderer_display(renderer, 0, time_now, 0.0f);
		ncx_renderer_poll(renderer);
	}

	ncx_model_destroy(&bong_model);
	ncx_model_destroy(&pistol_model);
	ncx_model_shader_destroy();

	ncx_materials_destroy(bong_materials, 3);
	ncx_materials_destroy(pistol_materials, 4);

	ncx_font_destroy(&font);
	ncx_font_shader_destroy();

	ncx_renderer_destroy(&renderer);
	return 0;
}

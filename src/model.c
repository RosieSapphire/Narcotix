#include "narcotix/model.h"
#include "narcotix/helpers.h"
#include "narcotix/glad/glad.h"
#include "narcotix/light_point.h"
#include "narcotix/shader.h"

#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>

#define POINT_LIGHT_MAX 32

#ifdef DEBUG
	#include "narcotix/debug.h"
	#include "rose_petal.h"
#endif

NCXShader ncx_model_shader_create_internal(const NCXLightPoint *lights,
		const uint8_t light_count, const char *file, const uint32_t line) {
	/* TODO: Make these parameters more customizable */
	NCXShader model_shader =
		ncx_shader_create_internal("res/shaders/model_vert.glsl",
				"res/shaders/model_frag.glsl", NULL, file, line);
	glUseProgram(model_shader);
	glUniform1i(glGetUniformLocation(model_shader,
				"light_points_count_current"), light_count);

	/* uploading light properties to shader */
	for(uint8_t i = 0; i < light_count; i++) {
		uint8_t j;
		char buffer[128] = { 0 };
		const NCXLightPoint light_cur = lights[i];
		const char *properties[7] = {
			"pos", "ambient_color", "diffuse_color", "specular_color",
			"constant", "linear", "quadratic"
		};

		const float *vectors[4] = {
			light_cur.pos, light_cur.ambient_color, light_cur.diffuse_color,
			light_cur.specular_color
		};

		const float values[3] = {
			light_cur.constant, light_cur.linear, light_cur.quadratic
		};

		for(j = 0; j < 4; j++) {
			sprintf(buffer, "light_points[%u].%s", i, properties[j]);
			glUniform3fv(glGetUniformLocation(model_shader, buffer), 1,
					vectors[j]);
		}

		for(; j < 7; j++) {
			sprintf(buffer, "light_points[%u].%s", i, properties[j]);
			glUniform1f(glGetUniformLocation(model_shader, buffer),
					values[j - 4]);
		}
	}

	return model_shader;
}

void ncx_model_shader_lights_update(const NCXShader shader,
		const NCXLightPoint *lights, const uint8_t light_count) {
	glUseProgram(shader);
	const char *properties[7] = { "pos", "ambient_color", "diffuse_color", "specular_color", "constant", "linear", "quadratic" };

	/* reset all lights to ensure no bullshittery */
	for(uint8_t i = 0; i < POINT_LIGHT_MAX; i++) {
		uint8_t j;
		char buffer[128] = { 0 };

		for(j = 0; j < 4; j++) {
			sprintf(buffer, "light_points[%u].%s", i, properties[j]);
			glUniform3fv(glGetUniformLocation(shader, buffer), 1,
					GLM_VEC3_ZERO);
		}

		for(; j < 7; j++) {
			sprintf(buffer, "light_points[%u].%s", i, properties[j]);
			glUniform1f(glGetUniformLocation(shader, buffer), 0.0f);
		}
	}

	/* now import the new lights */
	for(uint8_t i = 0; i < light_count; i++) {
		uint8_t j;
		char buffer[128] = { 0 };
		const NCXLightPoint light_cur = lights[i];
		const float *vectors[4] = {
			light_cur.pos, light_cur.ambient_color, light_cur.diffuse_color,
			light_cur.specular_color
		};

		const float values[3] = {
			light_cur.constant, light_cur.linear, light_cur.quadratic
		};

		for(j = 0; j < 4; j++) {
			sprintf(buffer, "light_points[%u].%s", i, properties[j]);
			glUniform3fv(glGetUniformLocation(shader, buffer), 1, vectors[j]);
		}

		for(; j < 7; j++) {
			sprintf(buffer, "light_points[%u].%s", i, properties[j]);
			glUniform1f(glGetUniformLocation(shader, buffer), values[j - 4]);
		}
	}

	glUniform1i(glGetUniformLocation(shader, "light_points_count_current"),
			light_count);
}

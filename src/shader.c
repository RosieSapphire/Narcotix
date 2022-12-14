#include "ncx/shader.h"
#include "ncx/file.h"
#include <glad/gl.h>

#include <assert.h>
#include <stdio.h>

ncx_shader_t ncx_shader_create(const char *vert_path,
		const char *geom_path, const char *frag_path)
{
	uint32_t shaders[3];
	char *shader_sources[3];
	const char *paths[3] = {vert_path, geom_path, frag_path};
	ncx_shader_t shader_program = glCreateProgram();

	const char *shader_type_names[3] = {
		"Vertex", "Fragment", "Geometry"
	};

	for(uint8_t i = 0; i < 3; i++) {
		if(!geom_path && i == 1) {
			continue;
		}

		const uint32_t shader_types[3] = {
			GL_VERTEX_SHADER,
			GL_GEOMETRY_SHADER,
			GL_FRAGMENT_SHADER
		};

		shaders[i] = glCreateShader(shader_types[i]);
		shader_sources[i] = ncx_file_load(paths[i]);
		if(!shader_sources[i]) {
			fprintf(stderr, "SHADER ERROR: Couldn't load "
					"%s Shader code from file '%s'.\n",
					shader_type_names[i], paths[i]);
			assert(0);
		}

		const char *const *shader_src =
			(const char *const *)&shader_sources[i];

		glShaderSource(shaders[i], 1, shader_src, NULL);
		glCompileShader(shaders[i]);

		int32_t success;
		glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &success);
		char info_log[512];
		if(!success) {
			glGetShaderInfoLog(shaders[i], 512, NULL, info_log);
			fprintf(stderr, "SHADER ERROR: %s shader from"
					" '%s' fucked up: %s.\n",
					shader_type_names[i], paths[i],
					info_log);

			return 0;
		}

		glAttachShader(shader_program, shaders[i]);
		glDeleteShader(shaders[i]);
		ncx_file_unload(shader_sources[i]);
	}

	glLinkProgram(shader_program);

	return shader_program;
}

void ncx_shader_destroy(const ncx_shader_t shader)
{
	glDeleteProgram(shader);
}

void ncx_shader_use(const ncx_shader_t shader)
{
	glUseProgram(shader);
}

void ncx_shader_uniform_int(const ncx_shader_t shader,
		const char *uniform, const int value)
{
	glUniform1i(glGetUniformLocation(shader, uniform), value);
}

void ncx_shader_uniform_float(const ncx_shader_t shader,
		const char *uniform, const float value)
{
	glUniform1f(glGetUniformLocation(shader, uniform), value);
}

void ncx_shader_uniform_vec3(const ncx_shader_t shader,
		const char *uniform, const struct ncx_vec3 value)
{
	glUniform3f(glGetUniformLocation(shader, uniform),
			value.x, value.y, value.z);
}

void ncx_shader_uniform_mat4(const ncx_shader_t shader,
		const char *uniform, const struct ncx_mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(shader, uniform),
			1, GL_FALSE, (const float *)value.mat);
}

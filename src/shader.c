#include "narcotix/shader.h"

#include <stdio.h>
#include <stdlib.h>
#include "narcotix/file.h"

#include "narcotix/glad/glad.h"
#include <GLFW/glfw3.h>

#ifdef DEBUG
#include "narcotix/debug.h"
#endif

NCXShader ncx_shader_create_internal(const char *vert_path,
		const char *geom_path, const char *frag_path,
		const char *file, const uint32_t line) {
	uint32_t shaders[3];
	char *shader_sources[3];
	const char *paths[3] = {vert_path, geom_path, frag_path};
	NCXShader shader_program = glCreateProgram();
	for(uint8_t i = 0; i < 3; i++) {
		if(!geom_path && i == 1) {
			continue;
		}

		const uint32_t shader_types[3] = {
			GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER
		};

		shaders[i] = glCreateShader(shader_types[i]);
		shader_sources[i] =
			ncx_file_load_contents_internal(paths[i], file, line);
		#ifdef DEBUG
		const char *shader_type_names[3] = {
			"Vertex", "Fragment", "Geometry"
		};

		if(!shader_sources[i]) {
			fprintf(stderr, "%sNARCOTIX::SHADER::ERROR: %sCouldn't find "
					"%s Shader code from file %s'%s'%s. %s(Caused at '%s'"
					" line %i)\n", D_COLOR_RED, D_COLOR_YELLOW,
					shader_type_names[i], D_COLOR_GREEN, paths[i],
					D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
			return 0;
		}
		#endif

		glShaderSource(shaders[i], 1,
				(const char * const *)&shader_sources[i], NULL);
		glCompileShader(shaders[i]);

		#ifdef DEBUG
		int32_t success;
		glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &success);
		char info_log[512];
		if(!success) {
			glGetShaderInfoLog(shaders[i], 512, NULL, info_log);
			fprintf(stderr, "%sNARCOTIX::SHADER::ERROR: %s%s shader from"
					" %s'%s'%s fucked up: %s%s(Caused at '%s' line %i)\n",
					D_COLOR_RED, D_COLOR_YELLOW, shader_type_names[i],
					D_COLOR_GREEN, paths[i], D_COLOR_YELLOW, info_log,
					D_COLOR_DEFAULT, file, line);
			glfwTerminate();
			return 0;
		}
		#endif

		glAttachShader(shader_program, shaders[i]);
		glDeleteShader(shaders[i]);
		ncx_file_unload_contents(shader_sources[i]);
	}

	glLinkProgram(shader_program);

	#ifdef DEBUG
	printf("%sNARCOTIX::SHADER::CREATE: %sShader from files"
			"%s'%s'%s%s %s'%s'%s", D_COLOR_GREEN, D_COLOR_YELLOW,
			D_COLOR_GREEN, paths[0], D_COLOR_YELLOW,
			(geom_path != NULL) ? "," : " and", D_COLOR_GREEN, paths[1],
			D_COLOR_YELLOW);

	if(geom_path) {
		printf(" and %s'%s'%s", D_COLOR_GREEN, paths[2], D_COLOR_YELLOW);
	}

	printf(" was successfully imported. %s(Caused at '%s' line %i)\n",
			D_COLOR_DEFAULT, file, line);
	#endif

	return shader_program;
}

void ncx_shader_destroy_internal(const NCXShader shader, const char *file,
		const uint32_t line) {
	#ifdef DEBUG
	printf("%sNARCOTIX::SHADER::DESTROY: %sShader from ID %s%i%s has been "
			"successfully destroyed. %s(Caused at '%s' line %i)\n",
			D_COLOR_GREEN, D_COLOR_YELLOW, D_COLOR_GREEN, shader,
			D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
	#endif
	glDeleteTextures(1, &shader);
}

void ncx_shader_use(const NCXShader shader) {
	glUseProgram(shader);
}

void ncx_shader_uniform_int(const NCXShader shader, const char *uniform,
		const int value) {
	glUniform1i(glGetUniformLocation(shader, uniform), value);
}

void ncx_shader_uniform_float(const NCXShader shader, const char *uniform,
		const float value) {
	glUniform1f(glGetUniformLocation(shader, uniform), value);
}

void ncx_shader_uniform_vec3(const NCXShader shader, const char *uniform,
		const vec3 value) {
	glUniform3fv(glGetUniformLocation(shader, uniform), 1, value);
}

void ncx_shader_uniform_mat4(const NCXShader shader, const char *uniform,
		const mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(shader, uniform),
			1, GL_FALSE, (const float *)value);
}

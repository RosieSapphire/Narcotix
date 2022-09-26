#include "narcotix/shader.h"

#include <stdio.h>
#include <stdlib.h>
#include "narcotix/file.h"

#include "narcotix/glad/glad.h"
#include <GLFW/glfw3.h>

#ifdef DEBUG
	#include "narcotix/debug.h"
#endif

NCXShader ncx_shader_create_internal(const char *vert_path, const char *frag_path, const char *file, const uint32_t line) {
	const char *paths[2] = {vert_path, frag_path};
	const uint32_t shader_types[2] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER};
	char *shader_sources[2];
	uint32_t shaders[2];
	NCXShader shader_program;

	/* creating shader program and binding vert and frag */
	shader_program = glCreateProgram();
	for(uint8_t i = 0; i < 2; i++) {
		#ifdef DEBUG
			const char *shader_type_names[2] = {"Vertex", "Fragment"};
			char info_log[512];
		#endif
		int32_t success;
		shaders[i] = glCreateShader(shader_types[i]);
		shader_sources[i] = ncx_file_load_contents_internal(paths[i], file, line);
		#ifdef DEBUG
			if(!shader_sources[i]) {
				fprintf(stderr, "%sNARCOTIX::SHADER::ERROR: %sCouldn't find %s Shader code from file %s'%s'%s. %s(Caused at '%s' line %i)\n",
						D_COLOR_RED, D_COLOR_YELLOW, shader_type_names[i], D_COLOR_GREEN, paths[i], D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
				return 0;
			} else {
				printf("%sNARCOTIX::SHADER::CREATE: %s%s Shader code from file %s'%s'%s was successfully loaded. %s(Caused at '%s' line %i)\n",
						D_COLOR_GREEN, D_COLOR_YELLOW, shader_type_names[i], D_COLOR_GREEN, paths[i], D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
			}
		#endif

		glShaderSource(shaders[i], 1, (const char * const *)&shader_sources[i], NULL);
		glCompileShader(shaders[i]);

		glGetShaderiv(shaders[i], GL_COMPILE_STATUS, &success);
		#ifdef DEBUG
			if(!success) {
				glGetShaderInfoLog(shaders[i], 512, NULL, info_log);
				fprintf(stderr, "%sNARCOTIX::SHADER::ERROR: %s%s shader from %s'%s'%s fucked up: %s%s(Caused at '%s' line %i)\n",
						D_COLOR_RED, D_COLOR_YELLOW, shader_type_names[i], D_COLOR_GREEN, paths[i], D_COLOR_YELLOW, info_log, D_COLOR_DEFAULT, file, line);
				glfwTerminate();
				return 0;
			} else {
				printf("%sNARCOTIX::SHADER::CREATE: %s%s Shader code from file %s'%s'%s has not encountered any errors. %s(Caused at '%s' line %i)\n",
						D_COLOR_GREEN, D_COLOR_YELLOW, shader_type_names[i], D_COLOR_GREEN, paths[i], D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
			}
		#endif

		glAttachShader(shader_program, shaders[i]);
		glDeleteShader(shaders[i]);
		free(shader_sources[i]);
	}

	glLinkProgram(shader_program);
	return shader_program;
}

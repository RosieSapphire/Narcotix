#include "narcotix/shader.h"

#include <stdio.h>
#include <stdlib.h>
#include "narcotix/file.h"

#include "narcotix/glad/glad.h"
#include <GLFW/glfw3.h>

#ifdef DEBUG
	#include "narcotix/debug.h"
#endif

NCXShader ncx_shader_create_internal(const char *vert_path, const char *frag_path, const char *geom_path, const char *file, const uint32_t line) {
	const char *paths[3] = {vert_path, frag_path, geom_path};
	const uint32_t shader_types[3] = {GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER};
	char *shader_sources[3];
	uint32_t shaders[3];
	NCXShader shader_program;

	/* creating shader program and binding vert and frag */
	shader_program = glCreateProgram();
	for(uint8_t i = 0; i < 2 + (geom_path != NULL); i++) {
		#ifdef DEBUG
			const char *shader_type_names[3] = {"Vertex", "Fragment", "Geometry"};
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
			}
		#endif

		glAttachShader(shader_program, shaders[i]);
		glDeleteShader(shaders[i]);
		free(shader_sources[i]);
	}

	glLinkProgram(shader_program);
	#ifdef DEBUG
	{
		printf("%sNARCOTIX::SHADER::CREATE: %sShader from files %s'%s'%s%s %s'%s'%s", D_COLOR_GREEN, D_COLOR_YELLOW, D_COLOR_GREEN, paths[0], D_COLOR_YELLOW, (geom_path != NULL) ? "," : " and", D_COLOR_GREEN, paths[1], D_COLOR_YELLOW);
		if(geom_path) printf(" and %s'%s'%s", D_COLOR_GREEN, paths[2], D_COLOR_YELLOW);
		printf(" was successfully imported. %s(Caused at '%s' line %i)\n", D_COLOR_DEFAULT, file, line);
	}
	#endif
	return shader_program;
}

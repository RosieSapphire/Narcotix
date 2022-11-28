#ifndef __NCX_SHADER_H_
#define __NCX_SHADER_H_

#include <stdint.h>
#include <cglm/cglm.h>

typedef uint32_t NCXShader;
NCXShader ncx_shader_create_internal(const char *vert_path,
		const char *geom_path, const char *frag_path,
		const char *file, const uint32_t line);

#define ncx_shader_create(VERT_PATH, GEOM_PATH, FRAG_PATH) \
	ncx_shader_create_internal(VERT_PATH, GEOM_PATH, FRAG_PATH, \
			__FILE__, __LINE__)

void ncx_shader_destroy_internal(const NCXShader shader, const char *file,
		const uint32_t line);

#define ncx_shader_destroy(SHADER) \
	ncx_shader_destroy_internal(SHADER, __FILE__, __LINE__)

void ncx_shader_use(const NCXShader shader);

void ncx_shader_uniform_int(const NCXShader shader, const char *uniform,
		const int value);
void ncx_shader_uniform_float(const NCXShader shader, const char *uniform,
		const float value);
void ncx_shader_uniform_vec3(const NCXShader shader, const char *uniform,
		const vec3 value);
void ncx_shader_uniform_mat4(const NCXShader shader, const char *uniform,
		const mat4 value);

#endif

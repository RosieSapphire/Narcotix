#ifndef __NCX_SHADER_H_
#define __NCX_SHADER_H_

#include <cglm/cglm.h>

typedef uint32_t NCXShader;
NCXShader ncx_shader_create(const char *vert_path,
		const char *geom_path, const char *frag_path);
void ncx_shader_destroy(const NCXShader shader);

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

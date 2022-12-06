#ifndef __NCX_SHADER_H_
#define __NCX_SHADER_H_

#include <cglm/cglm.h>
#include "narcotix/vec3.h"

typedef uint32_t ncx_shader_t;
ncx_shader_t ncx_shader_create(const char *vert_path,
		const char *geom_path, const char *frag_path);
void ncx_shader_destroy(const ncx_shader_t shader);

void ncx_shader_use(const ncx_shader_t shader);
void ncx_shader_uniform_int(const ncx_shader_t shader, const char *uniform,
		const int value);
void ncx_shader_uniform_float(const ncx_shader_t shader, const char *uniform,
		const float value);
void ncx_shader_uniform_vec3(const ncx_shader_t shader, const char *uniform,
		const ncx_vec3_t value);
void ncx_shader_uniform_mat4(const ncx_shader_t shader, const char *uniform,
		const mat4 value);

#endif

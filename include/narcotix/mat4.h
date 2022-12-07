#ifndef __NCX_MAT4_H_
#define __NCX_MAT4_H_

#include "narcotix/vec3.h"

typedef struct {
	float mat[4][4];
} ncx_mat4_t;

#define ncx_mat4_0() ((ncx_mat4_t) {{ \
			{0, 0, 0, 0}, \
			{0, 0, 0, 0}, \
			{0, 0, 0, 0}, \
			{0, 0, 0, 0}, \
		}})

/* used for initializing identity matrix */
#define ncx_mat4_id() ((ncx_mat4_t) {{ \
			{1, 0, 0, 0}, \
			{0, 1, 0, 0}, \
			{0, 0, 1, 0}, \
			{0, 0, 0, 1}, \
		}})

ncx_mat4_t ncx_mat4_persp(float fov_deg, float aspect, float near, float far);
ncx_mat4_t ncx_mat4_ortho(float l, float r, float t, float b);
ncx_mat4_t ncx_mat4_translate(ncx_mat4_t mat, ncx_vec3_t vec);
ncx_mat4_t ncx_mat4_scale(ncx_mat4_t mat, ncx_vec3_t vec);
ncx_mat4_t ncx_mat4_scale_uni(ncx_mat4_t mat, float s);
ncx_mat4_t ncx_mat4_rotate(ncx_mat4_t mat, ncx_vec3_t axis, float angle_rad);
ncx_mat4_t ncx_mat4_mul(ncx_mat4_t a, ncx_mat4_t b);
void ncx_mat4_print(ncx_mat4_t m);

#endif

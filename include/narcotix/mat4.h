#ifndef __NCX_MAT4_H_
#define __NCX_MAT4_H_

#include "narcotix/vec3.h"

typedef struct {
	float a[4], b[4], c[4], d[4];
} ncx_mat4_t;

ncx_mat4_t ncx_mat4_identity(void);
void ncx_mat4_translate(ncx_mat4_t *mat, ncx_vec3_t vec);

#endif

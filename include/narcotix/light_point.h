#ifndef __NCX_LIGHT_POINT_H_
#define __NCX_LIGHT_POINT_H_

#include "narcotix/vec3.h"

typedef struct {
	ncx_vec3_t pos;
	ncx_vec3_t ambi_col;
	ncx_vec3_t diff_col;
	ncx_vec3_t spec_col;
	float constant;
	float linear;
	float quadratic;
} ncx_light_point_t;

ncx_light_point_t ncx_light_point_create(ncx_vec3_t pos, ncx_vec3_t ambi_col,
		ncx_vec3_t diff_col, ncx_vec3_t spec_col, const float constant,
		const float linear, const float quadratic);

#endif

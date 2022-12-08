#ifndef __NCX_LIGHT_POINT_H_
#define __NCX_LIGHT_POINT_H_

#include "narcotix/vec3.h"

struct ncx_light_point {
	struct ncx_vec3 pos;
	struct ncx_vec3 ambi_col;
	struct ncx_vec3 diff_col;
	struct ncx_vec3 spec_col;
	float constant;
	float linear;
	float quadratic;
};

struct ncx_light_point ncx_light_point_create(struct ncx_vec3 pos,
		struct ncx_vec3 ambi_col, struct ncx_vec3 diff_col,
		struct ncx_vec3 spec_col, const float constant,
		const float linear, const float quadratic);

#endif

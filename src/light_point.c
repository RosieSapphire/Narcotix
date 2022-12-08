#include "narcotix/light_point.h"
#include <string.h>

ncx_light_point_t ncx_light_point_create(ncx_vec3_t pos, ncx_vec3_t ambi_col,
		ncx_vec3_t diff_col, ncx_vec3_t spec_col, const float constant,
		const float linear, const float quadratic) {

	ncx_light_point_t light;
	light.pos = pos;
	light.ambi_col = ambi_col;
	light.diff_col = diff_col;
	light.spec_col = spec_col;
	light.constant = constant;
	light.linear = linear;
	light.quadratic = quadratic;

	return light;
}

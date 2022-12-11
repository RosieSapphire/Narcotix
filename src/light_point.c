#include "ncx/light_point.h"
#include <string.h>

struct ncx_light_point ncx_light_point_create(struct ncx_vec3 pos,
		struct ncx_vec3 ambi_col, struct ncx_vec3 diff_col,
		struct ncx_vec3 spec_col, const float constant,
		const float linear, const float quadratic)
{
	return (struct ncx_light_point) {
		.pos = pos,
		.ambi_col = ambi_col,
		.diff_col = diff_col,
		.spec_col = spec_col,
		.constant = constant,
		.linear = linear,
		.quadratic = quadratic,
	};
}

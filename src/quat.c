#include "ncx/quat.h"
#include "ncx/helpers.h"

#include <math.h>

struct ncx_vec4 ncx_quat_lerp(struct ncx_vec4 a, struct ncx_vec4 b, float t)
{
	t = ncx_clampf01(t);
	return (struct ncx_vec4) {
		ncx_lerp(a.x, b.x, t),
		ncx_lerp(a.y, b.y, t),
		ncx_lerp(a.z, b.z, t),
		ncx_lerp(a.w, b.w, t),
	};
}

struct ncx_vec4 ncx_quat_slerp(struct ncx_vec4 a, struct ncx_vec4 b, float t)
{
	t = ncx_clampf01(t);

	float cos_angle = ncx_quat_dot(a, b);
	if(fabsf(cos_angle) >= 1.0f) {
		return a;
	}

	if(cos_angle < 0.0f) {
		a = ncx_vec4_neg(a);
		cos_angle *= -1;
	}

	float sin_angle = sqrtf(1.0f - cos_angle * cos_angle);
	if(fabsf(sin_angle) < NCX_EPSILON) {
		return ncx_quat_lerp(a, b, t);
	}

	float angle = acosf(cos_angle);
	a = ncx_vec4_scale(a, sinf((1.0f - t) * angle));
	b = ncx_vec4_scale(b, sinf(t * angle));
	a = ncx_vec4_add(a, b);
	return ncx_vec4_scale(a, 1.0f / sin_angle);
}

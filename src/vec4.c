#include "ncx/vec4.h"

struct ncx_vec4 ncx_vec4_add(struct ncx_vec4 a, struct ncx_vec4 b)
{
	return ncx_vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

struct ncx_vec4 ncx_vec4_sub(struct ncx_vec4 a, struct ncx_vec4 b)
{
	return ncx_vec4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

struct ncx_vec4 ncx_vec4_scale(struct ncx_vec4 a, float s)
{
	return ncx_vec4(a.x * s, a.y * s, a.z * s, a.w * s);
}

struct ncx_vec4 ncx_vec4_neg(struct ncx_vec4 x)
{
	return ncx_vec4(-x.x, -x.y, -x.z, -x.w);
}

struct ncx_vec4 ncx_vec4_lerp(struct ncx_vec4 a, struct ncx_vec4 b, float t)
{
	return ncx_vec4(ncx_lerp(a.x, b.x, t),
			ncx_lerp(a.y, b.y, t),
			ncx_lerp(a.z, b.z, t),
			ncx_lerp(a.w, b.w, t));
}

struct ncx_vec4 ncx_vec4_lerpc(struct ncx_vec4 a, struct ncx_vec4 b, float t)
{
	return ncx_vec4_lerp(a, b, ncx_clampf01(t));
}

float ncx_vec4_dot(struct ncx_vec4 a, struct ncx_vec4 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float ncx_vec4_mag_inv(struct ncx_vec4 x)
{
	return ncx_quake_rsqrt(ncx_vec4_dot(x, x));
}

float ncx_vec4_mag(struct ncx_vec4 x)
{
	return 1.0f / ncx_vec4_mag_inv(x);
}

struct ncx_vec4 ncx_vec4_norm(struct ncx_vec4 x)
{
	return ncx_vec4_scale(x, ncx_vec4_mag_inv(x));
}

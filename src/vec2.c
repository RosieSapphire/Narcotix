#include "ncx/vec2.h"

struct ncx_vec2 ncx_vec2_add(struct ncx_vec2 a, struct ncx_vec2 b)
{
	return ncx_vec2(a.x + b.x, a.y + b.y);
}

struct ncx_vec2 ncx_vec2_sub(struct ncx_vec2 a, struct ncx_vec2 b)
{
	return ncx_vec2(a.x - b.x, a.y - b.y);
}

struct ncx_vec2 ncx_vec2_scale(struct ncx_vec2 a, float s)
{
	return ncx_vec2(a.x * s, a.y * s);
}

struct ncx_vec2 ncx_vec2_neg(struct ncx_vec2 x)
{
	return ncx_vec2(-x.x, -x.y);
}

struct ncx_vec2 ncx_vec2_lerp(struct ncx_vec2 a,
		struct ncx_vec2 b, float t)
{
	return ncx_vec2(ncx_lerp(a.x, b.x, t), ncx_lerp(a.y, b.y, t));
}

struct ncx_vec2 ncx_vec2_lerpc(struct ncx_vec2 a,
		struct ncx_vec2 b, float t)
{
	return ncx_vec2_lerp(a, b, ncx_clampf01(t));
}

float ncx_vec2_dot(struct ncx_vec2 a, struct ncx_vec2 b)
{
	return a.x * b.x + a.y * b.y;
}

float ncx_vec2_mag_inv(struct ncx_vec2 x)
{
	return ncx_quake_rsqrt(ncx_vec2_dot(x, x));
}

float ncx_vec2_mag(struct ncx_vec2 x)
{
	return 1.0f / ncx_vec2_mag_inv(x);
}

struct ncx_vec2 ncx_vec2_norm(struct ncx_vec2 x)
{
	return ncx_vec2_scale(x, ncx_vec2_mag_inv(x));
}

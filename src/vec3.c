#include "narcotix/vec3.h"

struct ncx_vec3 ncx_vec3_add(struct ncx_vec3 a, struct ncx_vec3 b)
{
	return ncx_vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

struct ncx_vec3 ncx_vec3_sub(struct ncx_vec3 a, struct ncx_vec3 b)
{
	return ncx_vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

struct ncx_vec3 ncx_vec3_scale(struct ncx_vec3 a, float s)
{
	return ncx_vec3(a.x * s, a.y * s, a.z * s);
}

struct ncx_vec3 ncx_vec3_neg(struct ncx_vec3 x)
{
	return ncx_vec3(-x.x, -x.y, -x.z);
}

struct ncx_vec3 ncx_vec3_cross(struct ncx_vec3 a, struct ncx_vec3 b)
{
	return ncx_vec3(a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
}

struct ncx_vec3 ncx_vec3_lerp(struct ncx_vec3 a, struct ncx_vec3 b, float t)
{
	return ncx_vec3(ncx_lerp(a.x, b.x, t),
			        ncx_lerp(a.y, b.y, t),
			        ncx_lerp(a.z, b.z, t));
}

struct ncx_vec3 ncx_vec3_lerpc(struct ncx_vec3 a, struct ncx_vec3 b, float t)
{
	return ncx_vec3_lerp(a, b, ncx_clampf01(t));
}

float ncx_vec3_dot(struct ncx_vec3 a, struct ncx_vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float ncx_vec3_mag_inv(struct ncx_vec3 x)
{
	return ncx_quake_rsqrt(ncx_vec3_dot(x, x));
}

float ncx_vec3_mag(struct ncx_vec3 x)
{
	return 1.0f / ncx_vec3_mag_inv(x);
}

struct ncx_vec3 ncx_vec3_norm(struct ncx_vec3 x)
{
	return ncx_vec3_scale(x, ncx_vec3_mag_inv(x));
}

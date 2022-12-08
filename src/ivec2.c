#include "narcotix/ivec2.h"

struct ncx_ivec2 ncx_ivec2_add(struct ncx_ivec2 a, struct ncx_ivec2 b)
{
	return ncx_ivec2(a.x + b.x, a.y + b.y);
}

struct ncx_ivec2 ncx_ivec2_sub(struct ncx_ivec2 a, struct ncx_ivec2 b)
{
	return ncx_ivec2(a.x - b.x, a.y - b.y);
}

struct ncx_ivec2 ncx_ivec2_scale(struct ncx_ivec2 a, float s)
{
	return ncx_ivec2(a.x * s, a.y * s);
}

struct ncx_ivec2 ncx_ivec2_neg(struct ncx_ivec2 x)
{
	return ncx_ivec2(-x.x, -x.y);
}

float ncx_ivec2_dot(struct ncx_ivec2 a, struct ncx_ivec2 b)
{
	return a.x * b.x + a.y * b.y;
}

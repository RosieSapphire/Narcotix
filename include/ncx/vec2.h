#ifndef __NCX_VEC2_H_
#define __NCX_VEC2_H_

#include "ncx/helpers.h"

struct ncx_vec2 {
	float x, y;
};

#define ncx_vec2(X, Y) ((struct ncx_vec2){X, Y})
#define NCX_VEC2_0     ncx_vec2(0, 0)
#define NCX_VEC2_1     ncx_vec2(1, 1)
#define NCX_VEC2_X     ncx_vec2(1, 0)
#define NCX_VEC2_Y     ncx_vec2(0, 1)

struct ncx_vec2 ncx_vec2_add(struct ncx_vec2 a, struct ncx_vec2 b);
struct ncx_vec2 ncx_vec2_sub(struct ncx_vec2 a, struct ncx_vec2 b);
struct ncx_vec2 ncx_vec2_scale(struct ncx_vec2 a, float s);
struct ncx_vec2 ncx_vec2_neg(struct ncx_vec2 x);
struct ncx_vec2 ncx_vec2_lerp(struct ncx_vec2 a,
		struct ncx_vec2 b, float t);
struct ncx_vec2 ncx_vec2_lerpc(struct ncx_vec2 a,
		struct ncx_vec2 b, float t);
float ncx_vec2_dot(struct ncx_vec2 a, struct ncx_vec2 b);
float ncx_vec2_mag_inv(struct ncx_vec2 x);
float ncx_vec2_mag(struct ncx_vec2 x);
struct ncx_vec2 ncx_vec2_norm(struct ncx_vec2 x);

#endif

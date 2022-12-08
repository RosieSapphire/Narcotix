#ifndef __NCX_VEC4_H_
#define __NCX_VEC4_H_

#include "narcotix/helpers.h"

struct ncx_vec4 {
	float x, y, z, w;
};

#define ncx_vec4(X, Y, Z, W) ((struct ncx_vec4){X, Y, Z, W})
#define NCX_VEC4_0 ncx_vec4(0, 0, 0, 0)
#define NCX_VEC4_1 ncx_vec4(1, 1, 1, 1)
#define NCX_VEC4_X ncx_vec4(1, 0, 0, 0)
#define NCX_VEC4_Y ncx_vec4(0, 1, 0, 0)
#define NCX_VEC4_Z ncx_vec4(0, 0, 1, 0)
#define NCX_VEC4_W ncx_vec4(0, 0, 0, 1)

struct ncx_vec4 ncx_vec4_add(struct ncx_vec4 a, struct ncx_vec4 b);
struct ncx_vec4 ncx_vec4_sub(struct ncx_vec4 a, struct ncx_vec4 b);
struct ncx_vec4 ncx_vec4_scale(struct ncx_vec4 a, float s);
struct ncx_vec4 ncx_vec4_neg(struct ncx_vec4 x);
struct ncx_vec4 ncx_vec4_lerp(struct ncx_vec4 a, struct ncx_vec4 b, float t);
struct ncx_vec4 ncx_vec4_lerpc(struct ncx_vec4 a, struct ncx_vec4 b, float t);
float ncx_vec4_dot(struct ncx_vec4 a, struct ncx_vec4 b);
float ncx_vec4_mag_inv(struct ncx_vec4 x);
float ncx_vec4_mag(struct ncx_vec4 x);
struct ncx_vec4 ncx_vec4_norm(struct ncx_vec4 x);

#endif

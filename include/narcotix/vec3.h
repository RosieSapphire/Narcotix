#ifndef __NCX_VEC3_H_
#define __NCX_VEC3_H_

#include "narcotix/helpers.h"

struct ncx_vec3 {
	float x, y, z;
};

#define ncx_vec3(X, Y, Z) ((struct ncx_vec3){X, Y, Z})
#define NCX_VEC3_0 ncx_vec3(0, 0, 0)
#define NCX_VEC3_1 ncx_vec3(1, 1, 1)
#define NCX_VEC3_X ncx_vec3(1, 0, 0)
#define NCX_VEC3_Y ncx_vec3(0, 1, 0)
#define NCX_VEC3_Z ncx_vec3(0, 0, 1)

struct ncx_vec3 ncx_vec3_add(struct ncx_vec3 a, struct ncx_vec3 b);
struct ncx_vec3 ncx_vec3_sub(struct ncx_vec3 a, struct ncx_vec3 b);
struct ncx_vec3 ncx_vec3_scale(struct ncx_vec3 a, float s);
struct ncx_vec3 ncx_vec3_neg(struct ncx_vec3 x);
struct ncx_vec3 ncx_vec3_cross(struct ncx_vec3 a, struct ncx_vec3 b);
struct ncx_vec3 ncx_vec3_lerp(struct ncx_vec3 a, struct ncx_vec3 b, float t);
struct ncx_vec3 ncx_vec3_lerpc(struct ncx_vec3 a, struct ncx_vec3 b, float t);
float ncx_vec3_dot(struct ncx_vec3 a, struct ncx_vec3 b);
float ncx_vec3_mag_inv(struct ncx_vec3 x);
float ncx_vec3_mag(struct ncx_vec3 x);
struct ncx_vec3 ncx_vec3_norm(struct ncx_vec3 x);

#endif

#ifndef __NCX_MAT4_H_
#define __NCX_MAT4_H_

#include "ncx/vec4.h"
#include "ncx/vec3.h"

struct ncx_mat4 {
	float mat[4][4];
};

#define NCX_MAT4_0 ((struct ncx_mat4) {{ \
			{0, 0, 0, 0}, \
			{0, 0, 0, 0}, \
			{0, 0, 0, 0}, \
			{0, 0, 0, 0}, \
		}})

#define NCX_MAT4_ID ((struct ncx_mat4) {{ \
			{1, 0, 0, 0}, \
			{0, 1, 0, 0}, \
			{0, 0, 1, 0}, \
			{0, 0, 0, 1}, \
		}})

struct ncx_mat4 ncx_mat4_persp(float fov_deg,
		float aspect, float near, float far);

struct ncx_mat4 ncx_mat4_ortho(float l, float r, float t, float b);
struct ncx_mat4 ncx_mat4_trans(struct ncx_mat4 mat, struct ncx_vec3 vec);
struct ncx_mat4 ncx_mat4_trans_x(struct ncx_mat4 mat, float val);
struct ncx_mat4 ncx_mat4_trans_y(struct ncx_mat4 mat, float val);
struct ncx_mat4 ncx_mat4_trans_z(struct ncx_mat4 mat, float val);
struct ncx_mat4 ncx_mat4_scale(struct ncx_mat4 mat, struct ncx_vec3 vec);
struct ncx_mat4 ncx_mat4_scale_uni(struct ncx_mat4 mat, float s);

struct ncx_mat4 ncx_mat4_rotate(struct ncx_mat4 mat,
		struct ncx_vec3 axis, float angle_rad);

struct ncx_mat4 ncx_mat4_rotate_x(struct ncx_mat4 mat, float angle_rad);
struct ncx_mat4 ncx_mat4_rotate_y(struct ncx_mat4 mat, float angle_rad);
struct ncx_mat4 ncx_mat4_rotate_z(struct ncx_mat4 mat, float angle_rad);
struct ncx_mat4 ncx_mat4_mul(struct ncx_mat4 a, struct ncx_mat4 b);
struct ncx_mat4 ncx_mat4_from_quat(struct ncx_vec4 quat);
struct ncx_mat4 ncx_mat4_lookat(struct ncx_vec3 eye, struct ncx_vec3 focus);

void ncx_mat4_print(struct ncx_mat4 m);

#endif

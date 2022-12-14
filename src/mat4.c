#include "ncx/mat4.h"
#include "ncx/quat.h"
#include "ncx/helpers.h"

#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

struct ncx_mat4 ncx_mat4_persp(float fov_deg,
		float aspect, float near, float far)
{
	struct ncx_mat4 mat = NCX_MAT4_0;

	float fov_itan = 1.0f / tanf(fov_deg * NCX_TO_RAD * 0.5f);
	mat.mat[0][0] = fov_itan / aspect;
	mat.mat[1][1] = fov_itan;

	float z_range = 1.0f / (near - far);
	mat.mat[2][2] = (near + far) * z_range;
	mat.mat[2][3] = -1.0f;
	mat.mat[3][2] = 2.0f * near * far * z_range;

	return mat;
}

struct ncx_mat4 ncx_mat4_ortho(float l, float r, float t, float b)
{
	struct ncx_mat4 mat = NCX_MAT4_0;

	float rl = 1 / (r - l);
	float tb = 1 / (t - b);
	mat.mat[0][0] = 2 * rl;
	mat.mat[1][1] = 2 * tb;
	mat.mat[2][2] = -1;
	mat.mat[3][0] = -(r + l) * rl;
	mat.mat[3][1] = -(t + b) * tb;
	mat.mat[3][2] = 0;
	mat.mat[3][3] = 1;

	return mat;
}

struct ncx_mat4 ncx_mat4_trans(struct ncx_mat4 mat, struct ncx_vec3 vec)
{
	mat.mat[3][0] += vec.x;
	mat.mat[3][1] += vec.y;
	mat.mat[3][2] += vec.z;
	return mat;
}

struct ncx_mat4 ncx_mat4_trans_x(struct ncx_mat4 mat, float val)
{
	mat.mat[3][0] += val;
	return mat;
}

struct ncx_mat4 ncx_mat4_trans_y(struct ncx_mat4 mat, float val)
{
	mat.mat[3][1] += val;
	return mat;
}

struct ncx_mat4 ncx_mat4_trans_z(struct ncx_mat4 mat, float val)
{
	mat.mat[3][2] += val;
	return mat;
}

struct ncx_mat4 ncx_mat4_scale(struct ncx_mat4 mat, struct ncx_vec3 vec)
{
	mat.mat[0][0] *= vec.x;
	mat.mat[1][1] *= vec.y;
	mat.mat[2][2] *= vec.z;
	return mat;
}

struct ncx_mat4 ncx_mat4_scale_uni(struct ncx_mat4 mat, float s)
{
	return ncx_mat4_scale(mat, ncx_vec3(s, s, s));
}

struct ncx_mat4 ncx_mat4_rotate(struct ncx_mat4 mat,
		struct ncx_vec3 axis, float angle_rad)
{
	struct ncx_mat4 rot;
	memset(&rot, 0, sizeof(rot));

	const float angle_sin = sinf(angle_rad);
	const float angle_cos = cosf(angle_rad);
	rot.mat[0][0] = angle_cos + (axis.x * axis.x) * (1 - angle_cos);
	rot.mat[1][0] = axis.x * axis.y * (1 - angle_cos) - axis.z * angle_sin;
	rot.mat[2][0] = axis.x * axis.z * (1 - angle_cos) + axis.y * angle_sin;

	rot.mat[0][1] = axis.y * axis.x * (1 - angle_cos) + axis.z * angle_sin;
	rot.mat[1][1] = angle_cos + (axis.y * axis.y) * (1 - angle_cos);
	rot.mat[2][1] = axis.y * axis.z * (1 - angle_cos) - axis.x * angle_sin;

	rot.mat[0][2] = axis.z * axis.x * (1 - angle_cos) - axis.y * angle_sin;
	rot.mat[1][2] = axis.z * axis.y * (1 - angle_cos) + axis.x * angle_sin;
	rot.mat[2][2] = angle_cos + (axis.z * axis.z) * (1 - angle_cos);

	rot.mat[3][3] = 1.0f;

	return ncx_mat4_mul(mat, rot);
}

struct ncx_mat4 ncx_mat4_rotate_x(struct ncx_mat4 mat, float angle_rad)
{
	return ncx_mat4_rotate(mat, NCX_VEC3_X, angle_rad);
}

struct ncx_mat4 ncx_mat4_rotate_y(struct ncx_mat4 mat, float angle_rad)
{
	return ncx_mat4_rotate(mat, NCX_VEC3_Y, angle_rad);
}

struct ncx_mat4 ncx_mat4_rotate_z(struct ncx_mat4 mat, float angle_rad)
{
	return ncx_mat4_rotate(mat, NCX_VEC3_Z, angle_rad);
}

struct ncx_mat4 ncx_mat4_mul(struct ncx_mat4 a, struct ncx_mat4 b)
{
	struct ncx_mat4 c;
	memset(&c, 0, sizeof(c));

	for(uint8_t row = 0; row < 4; row++) {
		for(uint8_t col = 0; col < 4; col++) {
			for(uint8_t ind = 0; ind < 4; ind++) {
				c.mat[col][row] +=
					a.mat[ind][row] * b.mat[col][ind];
			}
		}
	}

	return c;
}

struct ncx_mat4 ncx_mat4_from_quat(struct ncx_vec4 quat)
{
	float norm = sqrtf(ncx_quat_dot(quat, quat));
	float s = 0.0f;
	if(norm > 0) {
 		s = (2.0f / norm);
	}

	return (struct ncx_mat4) {{
		{
			1.0f - (s * quat.y * quat.y) - (s * quat.z * quat.z),
			(s * quat.x * quat.y) + (s * quat.w * quat.z),
			(s * quat.x * quat.z) - (s * quat.w * quat.y),
			0.0f,
		},
		{
			(s * quat.x * quat.y) - (s * quat.w * quat.z),
			1.0f - (s * quat.x * quat.x) - (s * quat.z * quat.z),
			(s * quat.y * quat.z) + (s * quat.w * quat.x),
			0.0f,
		},
		{
			(s * quat.x * quat.z) + (s * quat.w * quat.y),
			(s * quat.y * quat.z) - (s * quat.w * quat.x),
			1.0f - (s * quat.x * quat.x) - (s * quat.y * quat.y),
			0.0f,
		},
		{
			0.0f,
			0.0f,
			0.0f,
			1.0f,
		},
	}};
}

struct ncx_mat4 ncx_mat4_lookat(struct ncx_vec3 eye, struct ncx_vec3 focus)
{
	struct ncx_vec3 f = ncx_vec3_norm(ncx_vec3_sub(focus, eye));
	struct ncx_vec3 s = ncx_vec3_norm(ncx_vec3_cross(NCX_VEC3_Y, f));
	struct ncx_vec3 u = ncx_vec3_cross(f, s);

	return (struct ncx_mat4) {{
		{s.x, u.x, -f.x, 0.0f},
		{s.y, u.y, -f.y, 0.0f},
		{s.z, u.z,  f.z, 0.0f},
		{
			-ncx_vec3_dot(s, eye),
			-ncx_vec3_dot(u, eye),
			 ncx_vec3_dot(f, eye),
			1.0f
		},
	}};
}

void ncx_mat4_print(struct ncx_mat4 m)
{
	for(uint8_t i = 0; i < 4; i++) {
		for(uint8_t j = 0; j < 4; j++) {
			printf("%.6f\t", m.mat[j][i]);
		}
		printf("\n");
	}
	printf("\n");
}

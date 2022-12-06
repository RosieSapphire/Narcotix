#include "narcotix/mat4.h"
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

ncx_mat4_t ncx_mat4_identity(void) {
	return (ncx_mat4_t) {{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1},
	}};
}

void ncx_mat4_translate(ncx_mat4_t *mat, ncx_vec3_t vec) {
	mat->mat[3][0] += vec.x;
	mat->mat[3][1] += vec.y;
	mat->mat[3][2] += vec.z;
}

void ncx_mat4_scale(ncx_mat4_t *mat, ncx_vec3_t vec) {
	mat->mat[0][0] *= vec.x;
	mat->mat[1][1] *= vec.y;
	mat->mat[2][2] *= vec.z;
}

void ncx_mat4_scale_uni(ncx_mat4_t *mat, float s) {
	ncx_mat4_scale(mat, ncx_vec3(s, s, s));
}

void ncx_mat4_rotate(ncx_mat4_t *mat, ncx_vec3_t axis, float angle_rad) {
	ncx_mat4_t rot;
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

	*mat = ncx_mat4_mul(*mat, rot);
}

ncx_mat4_t ncx_mat4_mul(ncx_mat4_t a, ncx_mat4_t b) {
	ncx_mat4_t c;
	memset(&c, 0, sizeof(c));

	for(uint8_t row = 0; row < 4; row++) {
		for(uint8_t col = 0; col < 4; col++) {
			for(uint8_t ind = 0; ind < 4; ind++) {
				c.mat[col][row] += a.mat[ind][row] * b.mat[col][ind];
			}
		}
	}

	return c;
}

void ncx_mat4_print(ncx_mat4_t m) {
	for(uint8_t i = 0; i < 4; i++) {
		for(uint8_t j = 0; j < 4; j++) {
			printf("%.2f\t", m.mat[j][i]);
		}
		printf("\n");
	}
	printf("\n");
}

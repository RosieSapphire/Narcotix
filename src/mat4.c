#include "narcotix/mat4.h"

ncx_mat4_t ncx_mat4_identity(void) {
	return (ncx_mat4_t) {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1},
	};
}

void ncx_mat4_translate(ncx_mat4_t *mat, ncx_vec3_t vec) {
	mat->d[0] += vec.x;
	mat->d[1] += vec.y;
	mat->d[2] += vec.z;
}

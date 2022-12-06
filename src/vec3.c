#include "narcotix/vec3.h"

ncx_vec3_t ncx_vec3(float x, float y, float z) {
	return (ncx_vec3_t){x, y, z};
}

ncx_vec3_t ncx_vec3_zero(void) {
	return (ncx_vec3_t){0, 0, 0};
}

ncx_vec3_t ncx_vec3_one(void) {
	return (ncx_vec3_t){1, 1, 1};
}

ncx_vec3_t ncx_vec3_x_up(void) {
	return (ncx_vec3_t){1, 0, 0};
}

ncx_vec3_t ncx_vec3_y_up(void) {
	return (ncx_vec3_t){0, 1, 0};
}

ncx_vec3_t ncx_vec3_z_up(void) {
	return (ncx_vec3_t){0, 0, 1};
}

ncx_vec3_t ncx_vec3_add(ncx_vec3_t a, ncx_vec3_t b) {
	return (ncx_vec3_t) {
		a.x + b.x,
		a.y + b.y,
		a.z + b.z,
	};
}

ncx_vec3_t ncx_vec3_sub(ncx_vec3_t a, ncx_vec3_t b) {
	return (ncx_vec3_t) {
		a.x - b.x,
		a.y - b.y,
		a.z - b.z,
	};
}

ncx_vec3_t ncx_vec3_scale(ncx_vec3_t x, float s) {
	return (ncx_vec3_t) {
		x.x * s,
		x.y * s,
		x.z * s,
	};
}

ncx_vec3_t ncx_vec3_cross(ncx_vec3_t a, ncx_vec3_t b) {
	return (ncx_vec3_t) {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x,
	};
}

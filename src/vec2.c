#include "narcotix/vec2.h"

ncx_vec2_t ncx_vec2(float x, float y) {
	return (ncx_vec2_t){x, y};
}

ncx_vec2_t ncx_vec2_zero(void) {
	return (ncx_vec2_t){0, 0};
}

ncx_vec2_t ncx_vec2_one(void) {
	return (ncx_vec2_t){1, 1};
}

ncx_vec2_t ncx_vec2_x_up(void) {
	return (ncx_vec2_t){1, 0};
}

ncx_vec2_t ncx_vec2_y_up(void) {
	return (ncx_vec2_t){0, 1};
}

ncx_vec2_t ncx_vec2_add(ncx_vec2_t a, ncx_vec2_t b) {
	return (ncx_vec2_t) {
		a.x + b.x,
		a.y + b.y,
	};
}

ncx_vec2_t ncx_vec2_sub(ncx_vec2_t a, ncx_vec2_t b) {
	return (ncx_vec2_t) {
		a.x - b.x,
		a.y - b.y,
	};
}

ncx_vec2_t ncx_vec2_scale(ncx_vec2_t x, float s) {
	return (ncx_vec2_t) {
		x.x * s,
		x.y * s,
	};
}

#include "narcotix/ivec2.h"

ncx_ivec2_t ncx_ivec2(int x, int y) {
	return (ncx_ivec2_t){x, y};
}

ncx_ivec2_t ncx_ivec2_zero(void) {
	return (ncx_ivec2_t){0, 0};
}

ncx_ivec2_t ncx_ivec2_one(void) {
	return (ncx_ivec2_t){1, 1};
}

ncx_ivec2_t ncx_ivec2_x_up(void) {
	return (ncx_ivec2_t){1, 0};
}

ncx_ivec2_t ncx_ivec2_y_up(void) {
	return (ncx_ivec2_t){0, 1};
}

ncx_ivec2_t ncx_ivec2_add(ncx_ivec2_t a, ncx_ivec2_t b) {
	return (ncx_ivec2_t) {
		a.x + b.x,
		a.y + b.y,
	};
}

ncx_ivec2_t ncx_ivec2_sub(ncx_ivec2_t a, ncx_ivec2_t b) {
	return (ncx_ivec2_t) {
		a.x - b.x,
		a.y - b.y,
	};
}

ncx_ivec2_t ncx_ivec2_scale(ncx_ivec2_t x, float s) {
	return (ncx_ivec2_t) {
		x.x * s,
		x.y * s,
	};
}

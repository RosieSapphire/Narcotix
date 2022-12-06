#ifndef __NCX_VEC2_H_
#define __NCX_VEC2_H_

typedef struct {
	float x, y;
} ncx_vec2_t;

ncx_vec2_t ncx_vec2(float x, float y);
ncx_vec2_t ncx_vec2_zero(void);
ncx_vec2_t ncx_vec2_one(void);
ncx_vec2_t ncx_vec2_x_up(void);
ncx_vec2_t ncx_vec2_y_up(void);
ncx_vec2_t ncx_vec2_add(ncx_vec2_t a, ncx_vec2_t b);
ncx_vec2_t ncx_vec2_sub(ncx_vec2_t a, ncx_vec2_t b);
ncx_vec2_t ncx_vec2_scale(ncx_vec2_t x, float s);

#endif

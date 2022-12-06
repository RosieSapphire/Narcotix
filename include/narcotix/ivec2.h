#ifndef __NCX_IVEC2_H_
#define __NCX_IVEC2_H_

typedef struct {
	float x, y;
} ncx_ivec2_t;

ncx_ivec2_t ncx_ivec2(int x, int y);
ncx_ivec2_t ncx_ivec2_zero(void);
ncx_ivec2_t ncx_ivec2_one(void);
ncx_ivec2_t ncx_ivec2_x_up(void);
ncx_ivec2_t ncx_ivec2_y_up(void);
ncx_ivec2_t ncx_ivec2_add(ncx_ivec2_t a, ncx_ivec2_t b);
ncx_ivec2_t ncx_ivec2_sub(ncx_ivec2_t a, ncx_ivec2_t b);
ncx_ivec2_t ncx_ivec2_scale(ncx_ivec2_t x, float s);

#endif

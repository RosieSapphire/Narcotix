#ifndef __NCX_VEC3_H_
#define __NCX_VEC3_H_

typedef struct {
	float x, y, z;
} ncx_vec3_t;

ncx_vec3_t ncx_vec3(float x, float y, float z);
ncx_vec3_t ncx_vec3_zero(void);
ncx_vec3_t ncx_vec3_one(void);
ncx_vec3_t ncx_vec3_x_up(void);
ncx_vec3_t ncx_vec3_y_up(void);
ncx_vec3_t ncx_vec3_z_up(void);
ncx_vec3_t ncx_vec3_add(ncx_vec3_t a, ncx_vec3_t b);
ncx_vec3_t ncx_vec3_sub(ncx_vec3_t a, ncx_vec3_t b);
ncx_vec3_t ncx_vec3_scale(ncx_vec3_t x, float s);
ncx_vec3_t ncx_vec3_cross(ncx_vec3_t a, ncx_vec3_t b);

#endif

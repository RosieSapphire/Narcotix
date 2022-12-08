#ifndef __NCX_RAY_H_
#define __NCX_RAY_H_

#include "narcotix/vec3.h"

uint8_t ncx_ray_triangle(struct ncx_vec3 eye,
		struct ncx_vec3 dir, struct ncx_vec3 *verts, float *distance);

#endif

#ifndef __NCX_VEC3_H_
#define __NCX_VEC3_H_

#include "narcotix/helpers.h"

typedef struct {
	float x, y, z;
} ncx_vec3_t;

#define NCX_VEC3_0 ((ncx_vec3_t){0, 0, 0})
#define NCX_VEC3_1 ((ncx_vec3_t){1, 1, 1})
#define NCX_VEC3_X ((ncx_vec3_t){1, 0, 0})
#define NCX_VEC3_Y ((ncx_vec3_t){0, 1, 0})
#define NCX_VEC3_Z ((ncx_vec3_t){0, 0, 1})

#define ncx_vec3(X, Y, Z)      ((ncx_vec3_t){X, Y, Z})
#define ncx_vec3_add(A, B)     ((ncx_vec3_t){A.x + B.x, A.y + B.y, A.z + B.z})
#define ncx_vec3_sub(A, B)     ((ncx_vec3_t){A.x - B.x, A.y - B.y, A.z - B.z})
#define ncx_vec3_scale(A, S)   ((ncx_vec3_t){A.x * S, A.y * S, A.z * S})
#define ncx_vec3_neg(A)        ((ncx_vec3_t){-A.x, -A.y, -A.z})
#define ncx_vec3_cross(A, B)   ((ncx_vec3_t){A.y * B.z - A.z * B.y, \
		                                     A.z * B.x - A.x * B.z, \
										     A.x * B.y - A.y * B.x,})
#define ncx_vec3_lerp(A, B, T) ((ncx_vec3_t){ncx_lerpf(A.x, B.x, T), \
		                                     ncx_lerpf(A.y, B.y, T), \
		                                     ncx_lerpf(A.z, B.z, T)})
#define ncx_vec3_lerpc(A, B, T)  ncx_vec3_lerp(A, B, ncx_clampf01(T))

#endif

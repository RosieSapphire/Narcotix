#ifndef __NCX_VEC4_H_
#define __NCX_VEC4_H_

typedef struct {
	float x, y, z, w;
} ncx_vec4_t;

#define NCX_VEC4_0 ((ncx_vec4_t){0, 0, 0, 0})
#define NCX_VEC4_1 ((ncx_vec4_t){1, 1, 1, 1})
#define NCX_VEC4_X ((ncx_vec4_t){1, 0, 0, 0})
#define NCX_VEC4_Y ((ncx_vec4_t){0, 1, 0, 0})
#define NCX_VEC4_Z ((ncx_vec4_t){0, 0, 1, 0})
#define NCX_VEC4_W ((ncx_vec4_t){0, 0, 0, 1})

#define ncx_vec4(X, Y, Z, W) ((ncx_vec4_t){X, Y, Z, W})
#define ncx_vec4_add(A, B)   ((ncx_vec4_t){A.x + B.x, A.y + B.y, \
		                                   A.z + B.z, A.w + B.w})
#define ncx_vec4_sub(A, B)   ((ncx_vec4_t){A.x - B.x, A.y - B.y, \
		                                   A.z - B.z, A.w - B.w})
#define ncx_vec4_scale(X, S) ((ncx_vec4_t){X.x * S, X.y * S, X.z * S, X.w * S})
#define ncx_vec4_neg(A)      ((ncx_vec4_t){-A.x, -A.y, -A.z, -A.w})

#endif

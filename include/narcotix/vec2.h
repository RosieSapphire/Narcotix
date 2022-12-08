#ifndef __NCX_VEC2_H_
#define __NCX_VEC2_H_

typedef struct {
	float x, y;
} ncx_vec2_t;

#define NCX_VEC2_0 ((ncx_vec2_t){0, 0)
#define NCX_VEC2_1 ((ncx_vec2_t){1, 1)
#define NCX_VEC2_X ((ncx_vec2_t){1, 0)
#define NCX_VEC2_Y ((ncx_vec2_t){0, 1)

#define ncx_vec2(X, Y)       ((ncx_vec2_t){X, Y})
#define ncx_vec2_add(A, B)   ((ncx_vec2_t){A.x + B.x, A.y + B.y})
#define ncx_vec2_sub(A, B)   ((ncx_vec2_t){A.x - B.x, A.y - B.y})
#define ncx_vec2_neg(A)      ((ncx_vec2_t){-A.x, -A.y})
#define ncx_vec2_scale(A, S) ((ncx_vec2_t){A.x * S, A.y * S})

#endif

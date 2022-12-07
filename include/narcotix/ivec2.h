#ifndef __NCX_IVEC2_H_
#define __NCX_IVEC2_H_

typedef struct {
	int x, y;
} ncx_ivec2_t;

#define ncx_ivec2(X, Y) ((ncx_ivec2_t){X, Y})

#define ncx_ivec2_0() ((ncx_ivec2_t){0, 0})
#define ncx_ivec2_1() ((ncx_ivec2_t){1, 1})
#define ncx_ivec2_x_up() ((ncx_ivec2_t){1, 0})
#define ncx_ivec2_y_up() ((ncx_ivec2_t){0, 1})

#define ncx_ivec2_add(A, B) ((ncx_ivec2_t){A.x + B.x, A.y + B.y})
#define ncx_ivec2_sub(A, B) ((ncx_ivec2_t){A.x - B.x, A.y - B.y})
#define ncx_ivec2_scale(A, S) ((ncx_ivec2_t){A.x * S, A.y * S})

#endif

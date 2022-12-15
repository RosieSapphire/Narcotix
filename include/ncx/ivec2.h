#ifndef __NCX_IVEC2_H_
#define __NCX_IVEC2_H_

#include "ncx/helpers.h"

/**
 * @brief A 2-component vector with integers as their types.
 ****************************************************/
struct ncx_ivec2 {
	int x, y;
};

#define ncx_ivec2(X, Y) ((struct ncx_ivec2){X, Y})
#define NCX_IVEC2_0     ncx_ivec2(0, 0)
#define NCX_IVEC2_1     ncx_ivec2(1, 1)
#define NCX_IVEC2_X     ncx_ivec2(1, 0)
#define NCX_IVEC2_Y     ncx_ivec2(0, 1)

struct ncx_ivec2 ncx_ivec2_add(struct ncx_ivec2 a, struct ncx_ivec2 b);
struct ncx_ivec2 ncx_ivec2_sub(struct ncx_ivec2 a, struct ncx_ivec2 b);
struct ncx_ivec2 ncx_ivec2_scale(struct ncx_ivec2 a, float s);
struct ncx_ivec2 ncx_ivec2_neg(struct ncx_ivec2 x);
float ncx_ivec2_dot(struct ncx_ivec2 a, struct ncx_ivec2 b);

#endif

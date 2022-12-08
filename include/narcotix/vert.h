#ifndef __NCX_VERTEX_H_
#define __NCX_VERTEX_H_

#include "narcotix/vec3.h"
#include "narcotix/vec2.h"

struct ncx_vert {
	struct ncx_vec3 pos;
	struct ncx_vec3 norm;
	struct ncx_vec2 uv;
	struct ncx_vec3 tan;
	struct ncx_vec3 bitan;
};

#endif

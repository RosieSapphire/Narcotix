#ifndef __NCX_VERTEX_H_
#define __NCX_VERTEX_H_

#include <stdint.h>

typedef struct {
	float pos[3];
	float norm[3];
	float uv[2];
	float tan[3];
	float bitan[3];
} NCXVertex;

#endif

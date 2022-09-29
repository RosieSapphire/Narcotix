#ifndef NCX_VERTEX_H
#define NCX_VERTEX_H

#include <stdint.h>

typedef struct {
	float pos[3];
	float normal[3];
	float uv[2];
	float tangent[3];
	float bitangent[3];
	int32_t bone_ids[3];
	float weights[3];
} NCXVertex;

#endif

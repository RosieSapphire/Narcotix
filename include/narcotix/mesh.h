#ifndef NCX_MESH_H
#define NCX_MESH_H

#include "narcotix/vertex.h"
#include "narcotix/material.h"
#include "narcotix/bone.h"

typedef struct {
	NCXVertex *vertices;
	uint32_t *indices;
	NCXMaterial material;
	NCXBone *bones;

	uint32_t vao, vbo, ebo;
	uint32_t vertex_count, index_count, bone_count;
} NCXMesh;

NCXMesh ncx_mesh_create(NCXVertex *vertices, uint32_t *indices, NCXMaterial material, const uint32_t vertex_count, const uint32_t index_count);
void ncx_mesh_draw(NCXMesh m, uint32_t s);
void ncx_mesh_destroy(NCXMesh *m);

#endif

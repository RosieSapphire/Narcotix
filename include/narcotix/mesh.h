#ifndef __NCX_MESH_H_
#define __NCX_MESH_H_

#include "narcotix/vertex.h"
#include "narcotix/material.h"

enum MeshBufferIndex {
	VAO = 0,
	VBO,
	EBO,
	MESH_BUFFER_COUNT,
};

typedef struct {
	uint32_t vertex_count, index_count;
	NCXVertex *vertices;
	uint32_t *indices;
	NCXMaterial material;

	uint32_t buffers[MESH_BUFFER_COUNT];
} NCXMesh;

NCXMesh ncx_mesh_create(NCXVertex *vertices, uint32_t *indices,
		NCXMaterial material, const uint32_t vertex_count,
		const uint32_t index_count);
NCXMesh ncx_mesh_read(const char *path, NCXMaterial mat);
void ncx_mesh_write(NCXMesh *m, const char *path);
void ncx_mesh_draw(NCXMesh m, uint32_t s);
void ncx_mesh_destroy(NCXMesh *m);

#endif

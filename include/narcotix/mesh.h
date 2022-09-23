#ifndef NCX_MESH_H
#define NCX_MESH_H

#include "narcotix/vertex.h"
#include "narcotix/texture.h"
#include "narcotix/bone.h"

typedef struct {
	ncx_vertex_t *vertices;
	uint32_t *indices;
	ncx_texture_t *textures;
	ncx_bone_t *bones;

	uint32_t vao, vbo, ebo;
	uint32_t vertex_count, index_count, bone_count;
} ncx_mesh_t;

ncx_mesh_t ncx_mesh_create(ncx_vertex_t *vertices, uint32_t *indices, ncx_texture_t *textures, const uint32_t vertex_count, const uint32_t index_count);
void ncx_mesh_draw(ncx_mesh_t m, uint32_t s);
void ncx_mesh_destroy(ncx_mesh_t *m);

#endif

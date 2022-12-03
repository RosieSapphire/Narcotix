#ifndef __NCX_MESH_H_
#define __NCX_MESH_H_

#include "narcotix/vertex.h"
#include "narcotix/material.h"
#include "narcotix/shader.h"
#include "narcotix/animation.h"

#include <assimp/scene.h>

enum MeshBufferIndex {
	VAO = 0,
	VBO,
	EBO,
	MESH_BUFFER_COUNT,
};

typedef struct {
	uint32_t vertex_count, index_count;
	ncx_vertex_t *vertices;
	uint32_t *indices;
	ncx_material_t material;

	uint32_t buffers[MESH_BUFFER_COUNT];
} ncx_mesh_t;

ncx_mesh_t *ncx_meshes_create(const struct aiScene *scene,
		const ncx_material_t *materials,uint32_t *mesh_count);
void ncx_meshes_draw(const ncx_mesh_t *meshes, const uint32_t mesh_count,
		const ncx_shader_t shader, mat4 mat_base);
void ncx_meshes_draw_anim(const ncx_mesh_t *meshes, const uint32_t mesh_count,
		const ncx_shader_t shader, NCXAnimation anim, mat4 mat_base);

#endif

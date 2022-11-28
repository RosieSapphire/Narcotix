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
	NCXVertex *vertices;
	uint32_t *indices;
	NCXMaterial material;

	uint32_t buffers[MESH_BUFFER_COUNT];
} NCXMesh;

NCXMesh *ncx_meshes_create(const struct aiScene *scene,
		const NCXMaterial *materials,uint32_t *mesh_count);
void ncx_meshes_draw(const NCXMesh *meshes, const uint32_t mesh_count,
		const NCXShader shader, mat4 mat_base);
void ncx_meshes_draw_anim(const NCXMesh *meshes, const uint32_t mesh_count,
		const NCXShader shader, NCXAnimation anim, mat4 mat_base);

#endif

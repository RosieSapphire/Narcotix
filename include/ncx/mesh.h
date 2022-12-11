#ifndef __NCX_MESH_H_
#define __NCX_MESH_H_

#include "ncx/vert.h"
#include "ncx/material.h"
#include "ncx/shader.h"
#include "ncx/animation.h"
#include "ncx/mat4.h"

#include <assimp/scene.h>

/**
 * @brief Containes the different buffers for a single mesh.
 * Namely VAO, VBO and EBO. It also uses the last element as a count.
 ********************************************************************/
enum ncx_mesh_buffers {
	VAO = 0,
	VBO,
	EBO,
	MESH_BUFFER_COUNT,
};

/**
 * @brief A mesh, which containes vertices, indices,
 * GL buffers, and a single mateiral.
 ************************************/
struct ncx_mesh {
	uint32_t vert_count, index_count;
	struct ncx_vert *vertices;
	uint32_t *indices;
	struct ncx_material material;

	uint32_t buffers[MESH_BUFFER_COUNT];
};

struct ncx_mesh *ncx_meshes_create(const struct aiScene *scene,
		const struct ncx_material *materials,uint32_t *mesh_count);
void ncx_meshes_draw(const struct ncx_mesh *meshes, const uint32_t mesh_count,
		const ncx_shader_t shader, struct ncx_mat4 mat_base);
void ncx_meshes_draw_anim(const struct ncx_mesh *meshes,
		const uint32_t mesh_count, const ncx_shader_t shader,
		struct ncx_anim anim, struct ncx_mat4 mat_base);

#endif

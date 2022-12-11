#include "ncx/mesh.h"
#include "ncx/vec2.h"
#include <glad/gl.h>

#include <stdio.h>
#include <string.h>
#include <malloc.h>

struct ncx_mesh *ncx_meshes_create(const struct aiScene *scene,
		const struct ncx_material *materials,uint32_t *mesh_count)
{
	*mesh_count = scene->mNumMeshes;

	struct ncx_mesh *meshes =
		malloc(sizeof(struct ncx_mesh) * scene->mNumMeshes);

	for(size_t i = 0; i < scene->mNumMeshes; i++) {
		struct ncx_mesh *mesh_cur = meshes + i;
		mesh_cur->material = materials[i];
		mesh_cur->vertices = malloc(0);
		mesh_cur->indices = malloc(0);
		const struct aiMesh *ai_mesh_cur = scene->mMeshes[i];
		mesh_cur->vert_count = ai_mesh_cur->mNumVertices;

		mesh_cur->vertices = realloc(mesh_cur->vertices,
				mesh_cur->vert_count *
				sizeof(struct ncx_vert));

		for(size_t j = 0; j < mesh_cur->vert_count; j++) {
			mesh_cur->vertices[j].pos =
				*(struct ncx_vec3 *)&ai_mesh_cur->mVertices[j];

			mesh_cur->vertices[j].norm =
				*(struct ncx_vec3 *)&ai_mesh_cur->mNormals[j];

			mesh_cur->vertices[j].uv =
				*(struct ncx_vec2 *)
				&ai_mesh_cur->mTextureCoords[0][j];

			mesh_cur->vertices[j].tan =
				*(struct ncx_vec3 *)&ai_mesh_cur->mTangents[j];

			mesh_cur->vertices[j].bitan =
				*(struct ncx_vec3 *)
				&ai_mesh_cur->mBitangents[j];
		}

		mesh_cur->index_count = 0;
		mesh_cur->indices = realloc(mesh_cur->indices,
				ai_mesh_cur->mNumFaces * 3 * sizeof(uint32_t));
		for(size_t j = 0; j < ai_mesh_cur->mNumFaces; j++) {
			const struct aiFace face = ai_mesh_cur->mFaces[j];
			size_t k;
			for(   k = mesh_cur->index_count;
					k < mesh_cur->index_count +
					face.mNumIndices;
					k++) {

				mesh_cur->indices[k] =
					face.mIndices[k -
						mesh_cur->index_count];
			}

			mesh_cur->index_count = k;
		}

		glGenVertexArrays(1, &mesh_cur->buffers[VAO]);
		glBindVertexArray(mesh_cur->buffers[VAO]);

		glGenBuffers(1, &mesh_cur->buffers[VBO]);
		glBindBuffer(GL_ARRAY_BUFFER, mesh_cur->buffers[VBO]);

		glBufferData(GL_ARRAY_BUFFER, mesh_cur->vert_count *
				sizeof(struct ncx_vert),
				mesh_cur->vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
				sizeof(struct ncx_vert),
				(void *)offsetof(struct ncx_vert, pos));

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
				sizeof(struct ncx_vert),
				(void *)offsetof(struct ncx_vert, norm));

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
				sizeof(struct ncx_vert),
				(void *)offsetof(struct ncx_vert, uv));

		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
				sizeof(struct ncx_vert),
				(void *)offsetof(struct ncx_vert, tan));

		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE,
				sizeof(struct ncx_vert),
				(void *)offsetof(struct ncx_vert, bitan));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &mesh_cur->buffers[EBO]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh_cur->buffers[EBO]);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_cur->index_count,
				mesh_cur->indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	return meshes;
}

void ncx_meshes_draw(const struct ncx_mesh *meshes, const uint32_t mesh_count,
		const ncx_shader_t shader, struct ncx_mat4 mat_base)
{
	ncx_shader_use(shader);
	ncx_shader_uniform_mat4(shader, "model", mat_base);
	ncx_shader_uniform_int(shader, "material.tex_diff", 0);
	ncx_shader_uniform_int(shader, "material.tex_spec", 1);
	ncx_shader_uniform_int(shader, "material.tex_norm", 2);

	for(uint32_t i = 0; i < mesh_count; i++) {
		const struct ncx_mesh *const mesh_cur = meshes + i;
		glBindVertexArray(mesh_cur->buffers[VAO]);
		ncx_tex_use(mesh_cur->material.diff, 0);
		ncx_tex_use(mesh_cur->material.spec, 1);
		ncx_tex_use(mesh_cur->material.norm, 2);
		ncx_shader_uniform_float(shader, "material.shininess",
				mesh_cur->material.data.shininess);

		glDrawElements(GL_TRIANGLES, mesh_cur->index_count,
				GL_UNSIGNED_INT, mesh_cur->indices);
	}
}

void ncx_meshes_draw_anim(const struct ncx_mesh *meshes,
		const uint32_t mesh_count, const ncx_shader_t shader,
		struct ncx_anim anim, struct ncx_mat4 mat_base)
{
	struct ncx_mat4 mat_root = ncx_animation_get_matrix(anim, 0);

	ncx_shader_use(shader);
	ncx_shader_uniform_mat4(shader, "model", mat_root);
	for(uint32_t i = 0; i < mesh_count; i++) {
		const struct ncx_mesh *const mesh_cur = meshes + i;
		glBindVertexArray(mesh_cur->buffers[VAO]);

		struct ncx_mat4 mat_model;
		mat_model = ncx_animation_get_matrix(anim, i);

		if(i) {
			mat_model = ncx_mat4_mul(mat_root, mat_model);
		}
		mat_model = ncx_mat4_mul(mat_base, mat_model);

		ncx_shader_uniform_mat4(shader, "model", mat_model);

		ncx_tex_use(mesh_cur->material.diff, 0);
		ncx_tex_use(mesh_cur->material.spec, 1);
		ncx_tex_use(mesh_cur->material.norm, 2);
		ncx_shader_uniform_int(shader, "material.tex_diff", 0);
		ncx_shader_uniform_int(shader, "material.tex_spec", 1);
		ncx_shader_uniform_int(shader, "material.tex_norm", 2);

		ncx_shader_uniform_float(shader, "material.shininess",
				mesh_cur->material.data.shininess);

		glDrawElements(GL_TRIANGLES, mesh_cur->index_count,
				GL_UNSIGNED_INT, mesh_cur->indices);
	}
}

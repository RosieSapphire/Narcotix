#include "narcotix/mesh.h"

#include <stdio.h>
#include <string.h>
#include "narcotix/glad/glad.h"
#include <malloc.h>
#include <cglm/cglm.h>

NCXMesh *ncx_meshes_create(const struct aiScene *scene,
		const NCXMaterial *materials,uint32_t *mesh_count) {
	*mesh_count = scene->mNumMeshes;
	NCXMesh *meshes = malloc(sizeof(NCXMesh) * scene->mNumMeshes);
	for(size_t i = 0; i < scene->mNumMeshes; i++) {
		NCXMesh *mesh_cur = meshes + i;
		mesh_cur->material = materials[i];
		mesh_cur->vertices = malloc(0);
		mesh_cur->indices = malloc(0);
		const struct aiMesh *ai_mesh_cur = scene->mMeshes[i];
		mesh_cur->vertex_count = ai_mesh_cur->mNumVertices;
		mesh_cur->vertices = realloc(mesh_cur->vertices,
				mesh_cur->vertex_count * sizeof(NCXVertex));
		for(size_t j = 0; j < mesh_cur->vertex_count; j++) {
			memcpy(mesh_cur->vertices[j].pos,
					&ai_mesh_cur->mVertices[j].x, sizeof(vec3));
			memcpy(mesh_cur->vertices[j].norm,
					&ai_mesh_cur->mNormals[j].x, sizeof(vec3));
			memcpy(mesh_cur->vertices[j].uv,
					&ai_mesh_cur->mTextureCoords[0][j].x, sizeof(vec2));
			memcpy(mesh_cur->vertices[j].tan,
					&ai_mesh_cur->mTangents[j].x, sizeof(vec3));
			memcpy(mesh_cur->vertices[j].bitan,
					&ai_mesh_cur->mBitangents[j].x, sizeof(vec3));
		}

		mesh_cur->index_count = 0;
		mesh_cur->indices = realloc(mesh_cur->indices,
				ai_mesh_cur->mNumFaces * 3 * sizeof(uint32_t));
		for(size_t j = 0; j < ai_mesh_cur->mNumFaces; j++) {
			const struct aiFace face = ai_mesh_cur->mFaces[j];
			size_t  k;
			for(    k = mesh_cur->index_count;
					k < mesh_cur->index_count + face.mNumIndices;
					k++) {
				mesh_cur->indices[k] =
					face.mIndices[k - mesh_cur->index_count];
			}

			mesh_cur->index_count = k;
		}

		glGenVertexArrays(1, &mesh_cur->buffers[VAO]);
		glBindVertexArray(mesh_cur->buffers[VAO]);

		glGenBuffers(1, &mesh_cur->buffers[VBO]);
		glBindBuffer(GL_ARRAY_BUFFER, mesh_cur->buffers[VBO]);
		glBufferData(GL_ARRAY_BUFFER, mesh_cur->vertex_count *
				sizeof(NCXVertex), mesh_cur->vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
				sizeof(NCXVertex), (void *)offsetof(NCXVertex, pos));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
				sizeof(NCXVertex), (void *)offsetof(NCXVertex, norm));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
				sizeof(NCXVertex), (void *)offsetof(NCXVertex, uv));
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE,
				sizeof(NCXVertex), (void *)offsetof(NCXVertex, tan));
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE,
				sizeof(NCXVertex), (void *)offsetof(NCXVertex, bitan));
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

void ncx_meshes_draw(const NCXMesh *meshes, const uint32_t mesh_count,
		const NCXShader shader) {
	glUseProgram(shader);
	for(uint32_t i = 0; i < mesh_count; i++) {
		const NCXMesh *const mesh_cur = meshes + i;
		glBindVertexArray(mesh_cur->buffers[VAO]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh_cur->material.textures[0]);
		glUniform1i(glGetUniformLocation(shader, "material.tex_diffuse"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mesh_cur->material.textures[1]);
		glUniform1i(glGetUniformLocation(shader, "material.tex_specular"), 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, mesh_cur->material.textures[2]);
		glUniform1i(glGetUniformLocation(shader, "material.tex_normal"), 2);

		glUniform1f(glGetUniformLocation(shader, "material.shininess"),
				mesh_cur->material.shininess);

		glDrawElements(GL_TRIANGLES, mesh_cur->index_count,
				GL_UNSIGNED_INT, mesh_cur->indices);
	}
}

void ncx_meshes_draw_anim(const NCXMesh *meshes, const uint32_t mesh_count,
		const NCXShader shader, NCXAnimation anim, mat4 mat_base) {
	glUseProgram(shader);
	mat4 mat_root;
	ncx_animation_get_matrix(anim, 0, mat_root);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE,
			(float *)mat_root);
	for(uint32_t i = 0; i < mesh_count; i++) {
		const NCXMesh *const mesh_cur = meshes + i;
		glBindVertexArray(mesh_cur->buffers[VAO]);

		mat4 mat_model;
		ncx_animation_get_matrix(anim, i, mat_model);
		if(i) {
			glm_mat4_mul(mat_root, mat_model, mat_model);
		}
		glm_mat4_mul(mat_base, mat_model, mat_model);

		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE,
				(float *)mat_model);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh_cur->material.textures[0]);
		glUniform1i(glGetUniformLocation(shader, "material.tex_diffuse"), 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, mesh_cur->material.textures[1]);
		glUniform1i(glGetUniformLocation(shader, "material.tex_specular"), 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, mesh_cur->material.textures[2]);
		glUniform1i(glGetUniformLocation(shader, "material.tex_normal"), 2);

		glUniform1f(glGetUniformLocation(shader, "material.shininess"),
				mesh_cur->material.shininess);

		glDrawElements(GL_TRIANGLES, mesh_cur->index_count,
				GL_UNSIGNED_INT, mesh_cur->indices);
	}
}

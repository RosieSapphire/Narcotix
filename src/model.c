#include "narcotix/model.h"
#include "narcotix/helpers.h"
#include "narcotix/glad/glad.h"
#include "narcotix/light_point.h"
#include "narcotix/shader.h"

#include <assimp/cimport.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>

static ncx_shader_t model_shader;

void ncx_model_shader_create(const NCXLightPoint *lights, const uint8_t light_count) {
	/*
	const vec3 light_positions[] = {
		{-4.0f,  2.5f,  -3.0f},
		{-4.0f,  2.5f,   3.0f},
		{ 4.0f,  2.5f,   3.0f},
		{ 4.0f,  2.5f,   0.25f},
	
		{-13.0f, 8.0f,  6.0f},
		{-13.0f, 8.0f, -6.0f},
		{-21.0f, 8.0f,  6.0f},
	};
	*/

	/* initializing with default light properties */
	/*
	vec3 light_diffuse_color;
	vec3 light_ambient_color;
	glm_vec3_scale(GLM_VEC3_ONE, 0.5f, light_diffuse_color);
	glm_vec3_scale(GLM_VEC3_ONE, 0.1f, light_ambient_color);
	*/

	/* TODO: Make these parameters more customizable */
	model_shader = ncx_shader_create("res/shaders/model_vert.glsl", "res/shaders/model_frag.glsl");
	glUseProgram(model_shader);

	/* set material properties */
	/*
	glUniform3f(glGetUniformLocation(model_shader, "material.specular_color"), 0.5f, 0.5f, 0.5f);
	glUniform1f(glGetUniformLocation(model_shader, "material.shininess"), 32.0f);
	*/

	/* set light properties */
	/*
	glUniform3f(glGetUniformLocation(model_shader, "light_dir.dir"), 0.0f, 0.0f, -1.0f);
	glUniform3fv(glGetUniformLocation(model_shader, "light_dir.ambient_color"), 1, (const float *)light_ambient_color);
	glUniform3fv(glGetUniformLocation(model_shader, "light_dir.diffuse_color"), 1, (const float *)light_diffuse_color);
	glUniform3f(glGetUniformLocation(model_shader, "light_dir.specular_color"), 1.0f, 1.0f, 1.0f);
	*/

	glUniform1i(glGetUniformLocation(model_shader, "is_animated"), 0);
	glUniform1i(glGetUniformLocation(model_shader, "light_points_count_current"), light_count);

	/* uploading light properties to shader */
	/* TODO: Add support for directional lights as well */
	for(uint8_t i = 0; i < light_count; i++) {
		uint8_t j;
		char buffer[128] = { 0 };
		const NCXLightPoint light_cur = lights[i];
		const char *properties[7] = { "pos", "ambient_color", "diffuse_color", "specular_color", "constant", "linear", "quadratic" };
		const float *vectors[4] = { light_cur.pos, light_cur.ambient_color, light_cur.diffuse_color, light_cur.specular_color };
		const float values[3] = { light_cur.constant, light_cur.linear, light_cur.quadratic };

		for(j = 0; j < 4; j++) {
			sprintf(buffer, "light_points[%d].%s", i, properties[j]);
			glUniform3fv(glGetUniformLocation(model_shader, buffer), 1, vectors[j]);
		}

		/* TODO: maybe make this one a new for loop instead of leaching off the previous one */
		for(; j < 7; j++) {
			sprintf(buffer, "light_points[%d].%s", i, properties[j]);
			glUniform1f(glGetUniformLocation(model_shader, buffer), values[j - 4]);
		}
	}
}

void ncx_model_shader_set_matrix_projection(const mat4 projection) {
	glUseProgram(model_shader);
	glUniformMatrix4fv(glGetUniformLocation(model_shader, "projection"), 1, GL_FALSE, (const float *)projection);
}

void ncx_model_shader_set_matrix_view(const mat4 view) {
	glUseProgram(model_shader);
	glUniformMatrix4fv(glGetUniformLocation(model_shader, "view"), 1, GL_FALSE, (const float *)view);
}

void ncx_model_shader_set_matrix_model(const mat4 model) {
	glUseProgram(model_shader);
	glUniformMatrix4fv(glGetUniformLocation(model_shader, "model"), 1, GL_FALSE, (const float *)model);
}

void ncx_model_shader_set_render_layer(const uint8_t render_layer) {
	glUseProgram(model_shader);
	glUniform1i(glGetUniformLocation(model_shader, "render_layer"), render_layer);
}

void ncx_model_shader_set_params(const mat4 view, const mat4 projection, const vec3 view_pos, const float time_now, const float trip_intensity) {
	glUseProgram(model_shader);
	ncx_model_shader_set_matrix_view(view);
	ncx_model_shader_set_matrix_projection(projection);
	glUniform3fv(glGetUniformLocation(model_shader, "view_pos"), 1, (const float *)view_pos);
	glUniform1f(glGetUniformLocation(model_shader, "time"), time_now);
	glUniform1f(glGetUniformLocation(model_shader, "trip_intensity"), trip_intensity);
}

void aimesh_process_bone(const struct aiMesh *m) {
	// printf("Mesh: %s\n", m->mName.data);
	for(uint32_t i = 0; i < m->mNumBones; i++) {
		const struct aiBone *bone_current = m->mBones[i];
		// printf("\tBone %u: %s\n", i, bone_current->mName.data);
		for(uint32_t j = 0; j < bone_current->mNumWeights; j++) {
			// const struct aiVertexWeight *weight_current = &bone_current->mWeights[j];
			// printf("\t\tWeight: Vid: %i, Strength: %f\n", weight_current->mVertexId, (double)weight_current->mWeight);
		}
	}
}

ncx_model_t ncx_model_create(const char *path, NCXMaterial *materials) {
	ncx_model_t ncx_model;
	const struct aiScene *scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_RemoveRedundantMaterials);
	ncx_model.meshes = NULL;
	ncx_model.mesh_count = 0;

	#ifdef DEBUG
		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			printf("ERROR: %s\n", aiGetErrorString());
		}
	#endif

	ncx_model_process_node(&ncx_model, scene->mRootNode, scene, materials);
	return ncx_model;
}

void ncx_model_process_node(ncx_model_t *m, struct aiNode *node, const struct aiScene *scene, NCXMaterial *materials) {
	if(!m->mesh_count) {
		m->mesh_count = node->mNumMeshes;
		m->meshes = malloc(m->mesh_count * sizeof(ncx_mesh_t));
	} else {
		m->mesh_count += node->mNumMeshes;
		m->meshes = realloc(m->meshes, m->mesh_count * sizeof(ncx_mesh_t));
	}

	for(uint32_t i = m->mesh_count - node->mNumMeshes; i < m->mesh_count; i++) {
		const struct aiMesh *const mesh = scene->mMeshes[i];
		ncx_vertex_t *vertices = NULL;
		uint32_t *indices = NULL;

		vertices = malloc(mesh->mNumVertices * sizeof(ncx_vertex_t));
		for(uint32_t j = 0; j < mesh->mNumVertices; j++) {
			glm_vec3_copy((float *)&mesh->mVertices[j], vertices[j].pos);
			glm_vec3_copy((float *)&mesh->mNormals[j], vertices[j].normal);
			glm_ivec3_copy((ivec3){-1, -1, -1}, vertices[j].bone_ids);
			glm_vec3_copy(GLM_VEC3_ZERO, vertices[j].weights);

			if(mesh->mTextureCoords[0]) {
				vertices[j].uv[0] = mesh->mTextureCoords[0][j].x;
				vertices[j].uv[1] = mesh->mTextureCoords[0][j].y;
			} else {
				glm_vec2_copy(GLM_VEC2_ZERO, vertices[j].uv);
			}
		}

		indices = malloc(mesh->mNumFaces * 3 * sizeof(uint32_t));
		{
			uint32_t index_count = 0;
			for(uint32_t j = 0; j < mesh->mNumFaces; j++) {
				uint32_t k;
				const struct aiFace face = mesh->mFaces[j];
				for(k = index_count; k < index_count + face.mNumIndices; k++) {
					indices[k] = face.mIndices[k - index_count];
				}

				index_count = k;
			}
		}

		/* TODO: ADD BONE CODE HERE */
		aimesh_process_bone(mesh);

		m->meshes[i] = ncx_mesh_create(vertices, indices, materials[i], mesh->mNumVertices, mesh->mNumFaces * 3);
		free(indices);
		free(vertices);
	}

	for(uint32_t i = 0; i < node->mNumChildren; i++) {
		ncx_model_process_node(m, node->mChildren[i], scene, materials);
	}
}

void ncx_model_draw(ncx_model_t m, const uint8_t use_animation, const float *projection, const float *view, const float *model) {
	glUseProgram(model_shader);
	glUniform1i(glGetUniformLocation(model_shader, "is_animated"), use_animation);
	glUniformMatrix4fv(glGetUniformLocation(model_shader, "projection"), 1, GL_FALSE, projection);
	glUniformMatrix4fv(glGetUniformLocation(model_shader, "view"), 1, GL_FALSE, view);
	glUniformMatrix4fv(glGetUniformLocation(model_shader, "model"), 1, GL_FALSE, model);
	for(uint32_t i = 0; i < m.mesh_count; i++)
		ncx_mesh_draw(m.meshes[i], model_shader);
}

void ncx_model_destroy(ncx_model_t *m) {
	free(m->meshes);
}

void ncx_model_shader_destroy() {
	glDeleteProgram(model_shader);
}

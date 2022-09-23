#ifndef NCX_MODEL_H
#define NCX_MODEL_H

#include <assimp/scene.h>

#include "narcotix/mesh.h"
#include "shader.h"

typedef struct {
	ncx_mesh_t *meshes;
	uint32_t mesh_count;
	uint32_t padding;
} ncx_model_t;

void ncx_model_shader_create(void);
void ncx_model_shader_set_matrix_projection(const mat4 ncx_model_matrix);
void ncx_model_shader_set_matrix_view(const mat4 ncx_model_matrix);
void ncx_model_shader_set_matrix_model(const mat4 ncx_model_matrix);
void ncx_model_shader_set_render_layer(const uint8_t render_layer);
void ncx_model_shader_set_params(const mat4 view, const mat4 projection, const vec3 view_pos, const float time_now, const float trip_intensity);

ncx_model_t ncx_model_create(const char *path, ncx_texture_t *textures);
void ncx_model_process_node(ncx_model_t *m, struct aiNode *node, const struct aiScene *scene, ncx_texture_t *textures);
void aimesh_process_bone(const struct aiMesh *m);
void ncx_model_draw(ncx_model_t m, const uint8_t use_animation);
void ncx_model_destroy(ncx_model_t *m);

void ncx_model_shader_destroy(void);


#endif

#ifndef NCX_MODEL_H
#define NCX_MODEL_H

#include <assimp/scene.h>

#include "narcotix/mesh.h"
#include "narcotix/light_point.h"

typedef struct {
	ncx_mesh_t *meshes;
	uint32_t mesh_count;
	uint32_t padding;
} ncx_model_t;

void ncx_model_shader_create(const NCXLightPoint *lights, const uint8_t light_count);
void ncx_model_shader_set_matrix_projection(const mat4 ncx_model_matrix);
void ncx_model_shader_set_matrix_view(const mat4 ncx_model_matrix);
void ncx_model_shader_set_matrix_model(const mat4 ncx_model_matrix);
void ncx_model_shader_set_render_layer(const uint8_t render_layer);
void ncx_model_shader_set_params(const mat4 view, const mat4 projection, const vec3 view_pos, const float time_now, const float trip_intensity);

ncx_model_t ncx_model_create(const char *path, NCXMaterial *materials);
void ncx_model_process_node(ncx_model_t *m, struct aiNode *node, const struct aiScene *scene, NCXMaterial *materials);
void aimesh_process_bone(const struct aiMesh *m);
void ncx_model_draw(ncx_model_t m, const uint8_t use_animation, const float *projection, const float *view, const float *model);
void ncx_model_destroy(ncx_model_t *m);

void ncx_model_shader_destroy(void);


#endif

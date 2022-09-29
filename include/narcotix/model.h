#ifndef NCX_MODEL_H
#define NCX_MODEL_H

#include <assimp/scene.h>

#include "narcotix/mesh.h"
#include "narcotix/light_point.h"

typedef struct {
	NCXMesh *meshes;
	uint32_t mesh_count;
	uint32_t padding;
} NCXModel;

void ncx_model_shader_create_internal(const NCXLightPoint *lights, const uint8_t light_count, const char *file, const uint32_t line);
#define ncx_model_shader_create(LIGHTS, LIGHT_COUNT) ncx_model_shader_create_internal(LIGHTS, LIGHT_COUNT, __FILE__, __LINE__)

void ncx_model_shader_lights_update(const NCXLightPoint *lights, const uint8_t light_count);

void ncx_model_shader_set_matrix_projection(const mat4 ncx_model_matrix);
void ncx_model_shader_set_matrix_view(const mat4 ncx_model_matrix);
void ncx_model_shader_set_matrix_model(const mat4 ncx_model_matrix);
void ncx_model_shader_set_render_layer(const uint8_t render_layer);
void ncx_model_shader_set_params(const mat4 view, const mat4 projection, const vec3 view_pos, const float time_now, const float trip_intensity);

NCXModel ncx_model_create_internal(const char *path, NCXMaterial *materials, const char *file, const uint32_t line);
#define ncx_model_create(PATH, MATERIALS) ncx_model_create_internal(PATH, MATERIALS, __FILE__, __LINE__)
void ncx_model_process_node(NCXModel *m, struct aiNode *node, const struct aiScene *scene, NCXMaterial *materials);
void aimesh_process_bone(const struct aiMesh *m);
void ncx_model_draw(NCXModel m, const uint8_t use_animation);
void ncx_model_destroy(NCXModel *m);

void ncx_model_shader_destroy(void);


#endif

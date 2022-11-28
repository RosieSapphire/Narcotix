#ifndef __NCX_MODEL_H_
#define __NCX_MODEL_H_

#include <assimp/scene.h>

#include "narcotix/mesh.h"
#include "narcotix/light_point.h"
#include <cglm/cglm.h>

typedef struct {
	uint32_t mesh_count;
	NCXMesh *meshes;
	const struct aiScene *scene;
	uint32_t anim_count, anim_selected;
	NCXAnimation *anims;
} NCXModel;

NCXShader ncx_model_shader_create_internal(const NCXLightPoint *lights,
		const uint8_t light_count, const char *file, const uint32_t line);
#define ncx_model_shader_create(LIGHTS, LIGHT_COUNT) \
	ncx_model_shader_create_internal(LIGHTS, LIGHT_COUNT, __FILE__, __LINE__)
void ncx_model_shader_lights_update(const NCXShader shader,
		const NCXLightPoint *lights, const uint8_t light_count);
NCXModel ncx_model_create(const char *path, const NCXMaterial *materials,
		const uint8_t is_animated);
void ncx_model_draw(NCXModel model, const NCXShader shader, mat4 root);
void ncx_model_animation_set(NCXModel *model, const uint32_t anim_index);
void ncx_model_animation_update(NCXModel *model, const float dt,
		const uint8_t loop);

#endif

#ifndef __NCX_MODEL_H_
#define __NCX_MODEL_H_

#include "narcotix/mesh.h"
#include "narcotix/light_point.h"
#include "narcotix/mat4.h"

typedef struct {
	uint32_t mesh_count;
	ncx_mesh_t *meshes;
	const struct aiScene *scene;
	uint32_t anim_count, anim_selected;
	NCXAnimation *anims;
} ncx_model_t;

ncx_shader_t ncx_model_shader_create(const ncx_light_point_t *lights,
		const uint8_t light_count);
ncx_model_t ncx_model_create(const char *path, const ncx_material_t *materials,
		const uint8_t is_animated);
void ncx_model_destroy(ncx_model_t *model);

void ncx_model_shader_lights_update(const ncx_shader_t shader,
		const ncx_light_point_t *lights, const uint8_t light_count);
void ncx_model_draw(ncx_model_t model, const ncx_shader_t shader,
		ncx_mat4_t root);
void ncx_model_animation_set(ncx_model_t *model, const uint32_t anim_index);
void ncx_model_animation_update(ncx_model_t *model, const float dt,
		const uint8_t loop);


#endif

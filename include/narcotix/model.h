#ifndef __NCX_MODEL_H_
#define __NCX_MODEL_H_

#include "narcotix/mesh.h"
#include "narcotix/light_point.h"
#include "narcotix/mat4.h"

struct ncx_model {
	uint32_t mesh_count;
	struct ncx_mesh *meshes;
	const struct aiScene *scene;
	uint32_t anim_count, anim_selected;
	struct ncx_anim *anims;
};

ncx_shader_t ncx_model_shader_create(const struct ncx_light_point *lights,
		const uint8_t light_count);

struct ncx_model ncx_model_create(const char *path,
		const struct ncx_material *materials,
		const uint8_t is_animated);

void ncx_model_destroy(struct ncx_model *model);

void ncx_model_shader_lights_update(const ncx_shader_t shader,
		const struct ncx_light_point *lights,
		const uint8_t light_count);

void ncx_model_draw(struct ncx_model model,
		const ncx_shader_t shader, struct ncx_mat4 root);

void ncx_model_animation_set(struct ncx_model *model,
		const uint32_t anim_index);

void ncx_model_animation_update(struct ncx_model *model, const float dt,
		const uint8_t loop);

#endif

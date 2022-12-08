#ifndef __NCX_ANIMATION_H_
#define __NCX_ANIMATION_H_

#include "narcotix/mat4.h"
#include "narcotix/vec4.h"

#include <assimp/scene.h>

#define ANIM_FPS 60.0f

struct ncx_anim_channel {
	uint32_t mesh_index, tick_count;
	struct ncx_vec3 *pos_keys;
	struct ncx_vec4 *quat_keys;
};

struct ncx_anim {
	float timer;
	uint32_t channel_count;
	struct ncx_anim_channel *channels;
};

struct ncx_anim *ncx_animations_create(const struct aiScene *scene,
		uint32_t *count);

struct ncx_mat4 ncx_animation_get_matrix(const struct ncx_anim anim,
		uint32_t msh_id);

#endif

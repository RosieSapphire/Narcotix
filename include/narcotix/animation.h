#ifndef __NCX_ANIMATION_H_
#define __NCX_ANIMATION_H_

#include "narcotix/mat4.h"
#include "narcotix/vec4.h"

#include <assimp/scene.h>

#define ANIM_FPS 60.0f

typedef struct {
	uint32_t mesh_index, tick_count;
	ncx_vec3_t *pos_keys;
	ncx_vec4_t *quat_keys;
} NCXAnimChannel;

typedef struct {
	float timer;
	uint32_t channel_count;
	NCXAnimChannel *channels;
} NCXAnimation;

NCXAnimation *ncx_animations_create(const struct aiScene *scene,
		uint32_t *anim_count);
ncx_mat4_t ncx_animation_get_matrix(const NCXAnimation anim, uint32_t msh_id);

#endif

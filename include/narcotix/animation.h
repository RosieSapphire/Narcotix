#ifndef __NCX_ANIMATION_H_
#define __NCX_ANIMATION_H_

#include <cglm/cglm.h>
#include <assimp/scene.h>

typedef struct {
	uint32_t mesh_index, tick_count;
	vec3 *pos_keys;
	vec4 *quat_keys;
} NCXAnimChannel;

typedef struct {
	float timer;
	uint32_t channel_count;
	NCXAnimChannel *channels;
} NCXAnimation;

NCXAnimation *ncx_animations_create(const struct aiScene *scene,
		uint32_t *anim_count);
void ncx_animation_update(NCXAnimation *anim, const float dt);
void ncx_animation_get_matrix(const NCXAnimation anim, uint32_t mesh_index,
		mat4 out);

#endif

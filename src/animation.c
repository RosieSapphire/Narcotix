#include "narcotix/animation.h"
#include "narcotix/vec3.h"
#include "narcotix/quat.h"

#include <malloc.h>

NCXAnimation *ncx_animations_create(const struct aiScene *scene,
		uint32_t *anim_count) {
	*anim_count = scene->mNumAnimations - scene->mNumMeshes;
	NCXAnimation *anims = malloc(sizeof(NCXAnimation) * *anim_count);

	for(uint32_t i = 0; i < *anim_count; i++) {
		NCXAnimation *anim = anims + i;
		anim->timer = 0.0f;
		anim->channel_count = scene->mAnimations[i]->mNumChannels;
		anim->channels = malloc(sizeof(NCXAnimChannel) *
				anim->channel_count);
		for(uint32_t j = 0; j < anim->channel_count; j++) {
			const struct aiNodeAnim *ai_channel =
				scene->mAnimations[i]->mChannels[j];
			NCXAnimChannel *channel = &anim->channels[j];
			channel->tick_count = ai_channel->mNumPositionKeys;
			channel->pos_keys =
				malloc(sizeof(ncx_vec3_t) * channel->tick_count);
			channel->quat_keys =
				malloc(sizeof(ncx_vec4_t) * channel->tick_count);
			for(uint32_t k = 0; k < channel->tick_count; k++) {
				const struct aiVectorKey pos_key =
					ai_channel->mPositionKeys[k];

				channel->pos_keys[k] = *(ncx_vec3_t *)&pos_key.mValue;

				const struct aiQuatKey quat_key = ai_channel->mRotationKeys[k];
				ncx_vec4_t quat_corrected = {
					quat_key.mValue.x, quat_key.mValue.y,
					quat_key.mValue.z, quat_key.mValue.w,
				};

				channel->quat_keys[k] = quat_corrected;
			}
		}
	}

	return anims;
}

ncx_mat4_t ncx_animation_get_matrix(const NCXAnimation anim, uint32_t msh_id) {

	int anim_frame_a = (int)anim.timer;
	int anim_frame_b = (anim_frame_a + 1) %
		anim.channels->tick_count;
	float t = anim.timer - anim_frame_a;

	msh_id = anim.channel_count - 1 - msh_id;
	ncx_vec4_t anim_quat_lerped = ncx_quat_slerp(
			anim.channels[msh_id].quat_keys[anim_frame_a],
			anim.channels[msh_id].quat_keys[anim_frame_b], t);

	ncx_mat4_t model_rot = ncx_mat4_from_quat(anim_quat_lerped);
	ncx_vec3_t anim_pos_lerped = ncx_vec3_lerpc(
			anim.channels[msh_id].pos_keys[anim_frame_a],
			anim.channels[msh_id].pos_keys[anim_frame_b], t);

	ncx_mat4_t model_trans = ncx_mat4_translate(NCX_MAT4_ID, anim_pos_lerped);
	return ncx_mat4_mul(model_trans, model_rot);
}

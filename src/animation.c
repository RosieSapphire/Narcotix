#include "narcotix/animation.h"
#include "narcotix/vec3.h"
#include "narcotix/quat.h"

#include <malloc.h>

struct ncx_anim *ncx_animations_create(const struct aiScene *scene,
		uint32_t *anim_count) {
	*anim_count = scene->mNumAnimations - scene->mNumMeshes;
	struct ncx_anim *anims = malloc(sizeof(struct ncx_anim) * *anim_count);

	for(uint32_t i = 0; i < *anim_count; i++) {
		struct ncx_anim *anim = anims + i;
		anim->timer = 0.0f;
		anim->channel_count = scene->mAnimations[i]->mNumChannels;
		anim->channels = malloc(sizeof(struct ncx_anim_channel) *
				anim->channel_count);
		for(uint32_t j = 0; j < anim->channel_count; j++) {
			const struct aiNodeAnim *ai_channel =
				scene->mAnimations[i]->mChannels[j];
			struct ncx_anim_channel *channel = &anim->channels[j];
			channel->tick_count = ai_channel->mNumPositionKeys;
			channel->pos_keys =
				malloc(sizeof(struct ncx_vec3) *
						channel->tick_count);
			channel->quat_keys =
				malloc(sizeof(struct ncx_vec4) *
						channel->tick_count);
			for(uint32_t k = 0; k < channel->tick_count; k++) {
				const struct aiVectorKey pos_key =
					ai_channel->mPositionKeys[k];

				memcpy(&channel->pos_keys[k], &pos_key.mValue,
						sizeof(struct ncx_vec3));

				const struct aiQuatKey quat_key =
					ai_channel->mRotationKeys[k];

				struct ncx_vec4 quat_corrected = {
					quat_key.mValue.x, quat_key.mValue.y,
					quat_key.mValue.z, quat_key.mValue.w,
				};

				channel->quat_keys[k] = quat_corrected;
			}
		}
	}

	return anims;
}

struct ncx_mat4 ncx_animation_get_matrix(const struct ncx_anim anim,
		uint32_t msh_id)
{
	int anim_frame_a = (int)anim.timer;
	int anim_frame_b = (anim_frame_a + 1) %
		anim.channels->tick_count;
	float t = anim.timer - anim_frame_a;

	msh_id = anim.channel_count - 1 - msh_id;
	struct ncx_vec4 anim_quat_lerped = ncx_quat_slerp(
			anim.channels[msh_id].quat_keys[anim_frame_a],
			anim.channels[msh_id].quat_keys[anim_frame_b], t);

	struct ncx_mat4 model_rot = ncx_mat4_from_quat(anim_quat_lerped);
	struct ncx_vec3 anim_pos_lerped = ncx_vec3_lerpc(
			anim.channels[msh_id].pos_keys[anim_frame_a],
			anim.channels[msh_id].pos_keys[anim_frame_b], t);

	struct ncx_mat4 model_trans =
		ncx_mat4_trans(NCX_MAT4_ID, anim_pos_lerped);

	return ncx_mat4_mul(model_trans, model_rot);
}

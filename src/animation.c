#include "narcotix/animation.h"

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
			channel->pos_keys = malloc(sizeof(vec3) * channel->tick_count);
			channel->quat_keys = malloc(sizeof(vec4) * channel->tick_count);
			for(uint32_t k = 0; k < channel->tick_count; k++) {
				const struct aiVectorKey pos_key = ai_channel->mPositionKeys[k];
				memcpy(channel->pos_keys[k], &pos_key.mValue, sizeof(vec3));

				const struct aiQuatKey quat_key = ai_channel->mRotationKeys[k];
				vec4 quat_corrected = {
					quat_key.mValue.x, quat_key.mValue.y,
					quat_key.mValue.z, quat_key.mValue.w,
				};

				memcpy(channel->quat_keys[k], quat_corrected, sizeof(vec4));
			}
		}
	}

	return anims;
}

void ncx_animation_get_matrix(const NCXAnimation anim, uint32_t mesh_index, mat4 out) {
	int anim_frame_a = (int)anim.timer;
	int anim_frame_b = (anim_frame_a + 1) %
		anim.channels->tick_count;
	float t = anim.timer - anim_frame_a;

	mesh_index = anim.channel_count - 1 - mesh_index;
	vec4 anim_quat_lerped;
	glm_quat_slerp(
			anim.channels[mesh_index].quat_keys[anim_frame_a],
			anim.channels[mesh_index].quat_keys[anim_frame_b],
			t, anim_quat_lerped);

	mat4 model_rot;
	glm_quat_mat4(anim_quat_lerped, model_rot);

	vec3 anim_pos_lerped;
	glm_vec3_lerp(
			anim.channels[mesh_index].pos_keys[anim_frame_a],
			anim.channels[mesh_index].pos_keys[anim_frame_b],
			t, anim_pos_lerped);

	mat4 model_trans;
	glm_mat4_identity(model_trans);
	glm_translate(model_trans, anim_pos_lerped);
	glm_mat4_mul(model_trans, model_rot, out);
}

#ifndef __NCX_ANIMATION_H_
#define __NCX_ANIMATION_H_

#include "ncx/mat4.h"
#include "ncx/vec4.h"

#include <assimp/scene.h>

/**
 * @brief The default framerate of animations (will be interpolated)
 ************************************************************/
#define ANIM_FPS 60.0f 
		       

/**
 * @brief A single thread of animation, containing it's XYZ position, and 
 * XYZW quaternion rotation.
 ***************************/
struct ncx_anim_channel {
	uint32_t mesh_index, tick_count;
	struct ncx_vec3 *pos_keys;
	struct ncx_vec4 *quat_keys;
};

/**
 * @brief A mesh animation containing all the keyframes necessary for an
 * animation, as well as a timer.
 ********************************/
struct ncx_anim {
	float timer;
	uint32_t channel_count;
	struct ncx_anim_channel *channels;
};

/**
 * @brief Create a new animation from an Assimp scene.
 * @param scene An Assimp scene created from calling "aiImportFile"
 * @param count Output for the number of animations there are in the scene
 *************************************************************************/
struct ncx_anim *ncx_animations_create(const struct aiScene *scene,
		uint32_t *count);

/**
 * @brief Calculates an offset matrix for a model based on it's current
 * animation timer, using interpolated position and rotation keyframes.
 * @param anim The animation in question we're testing for.
 * @param msh_id The index of the mesh we want to update this animation
 * for
 *****/
struct ncx_mat4 ncx_animation_get_matrix(const struct ncx_anim anim,
		uint32_t msh_id);

#endif

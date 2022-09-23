#include "narcotix/bone.h"

ncx_bone_t ncx_bone_create(const char *name, const int32_t id, mat4 transform_bind, const uint32_t children_count) {
	ncx_bone_t bone;
	bone.name = name;
	bone.id = id;
	glm_mat4_copy(transform_bind, bone.transform_bind);
	bone.children_count = children_count;
	bone.children = calloc(children_count, sizeof(ncx_bone_t));
	return bone;
}

void ncx_bone_destroy(ncx_bone_t *bone) {
	free(bone->children);
}


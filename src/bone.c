#include "narcotix/bone.h"

NCXBone ncx_bone_create(const char *name, const int32_t id, mat4 transform_bind, const uint32_t children_count) {
	NCXBone bone;
	bone.name = name;
	bone.id = id;
	glm_mat4_copy(transform_bind, bone.transform_bind);
	bone.children_count = children_count;
	bone.children = calloc(children_count, sizeof(NCXBone));
	return bone;
}

void ncx_bone_destroy(NCXBone *bone) {
	free(bone->children);
}


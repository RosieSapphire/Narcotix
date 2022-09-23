#ifndef NCX_BONE_H
#define NCX_BONE_H

#include "narcotix/cglm/cglm.h"

typedef struct {
	const char *name;
	int32_t id;
	uint32_t pad0;
	mat4 offset;

	struct bone_t *children;
	struct bone_t *parent;
	uint32_t children_count;
	uint32_t pad1;
	uint64_t pad2;
	mat4 transform_animated;
	mat4 transform_bind;
} ncx_bone_t;

ncx_bone_t ncx_bone_create(const char *name, const int32_t id, mat4 transform_bind, const uint32_t children_count);
void ncx_bone_destroy(ncx_bone_t *bone);

#endif

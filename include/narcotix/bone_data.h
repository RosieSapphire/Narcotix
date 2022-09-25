#ifndef NCX_BONE_DATA_H
#define NCX_BONE_DATA_H

#include <stdint.h>

#define MAX_BONES_PER_VERTEX 4

typedef struct {
	uint32_t bone_ids[MAX_BONES_PER_VERTEX];
	float weights[MAX_BONES_PER_VERTEX];
} NCXBoneData;

NCXBoneData bone_data_create(void);
void bone_data_add(NCXBoneData *data, const uint32_t bone_id, const float weight);

#endif

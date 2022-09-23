#include "bone_data.h"

#include <string.h>
#include "helpers.h"

bone_data_t bone_data_create(void) {
	bone_data_t data;
	memset(data.bone_ids, 0, MAX_BONES_PER_VERTEX * sizeof(uint32_t));
	memset(data.weights, 0.0f, MAX_BONES_PER_VERTEX * sizeof(float));
	printfvi((const int32_t *)data.bone_ids, 4);
	printfv(data.weights, 4);
	return data;
}

void bone_data_add(bone_data_t *data, const uint32_t bone_id, const float weight) {
	for(uint32_t i = 0; i < MAX_BONES_PER_VERTEX; i++) {
		if(data->weights[i] == 0.0f) {
			data->bone_ids[i] = bone_id;
			data->weights[i] = weight;
			printf("Bone: %i, Weight: %f, Index %i\n", bone_id, (double)weight, i);
		}
	}
}

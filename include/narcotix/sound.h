#ifndef __NCX_SOUND_H_
#define __NCX_SOUND_H_

#include "narcotix/vec3.h"

typedef enum {
	SP_GAIN = 0x100A,
	SP_PITCH = 0x1003,
} ncx_sound_props_t;

typedef struct {
	uint32_t source;
	uint32_t *delay_sources;
	uint32_t *buffers;
	uint8_t buffer_count;
	uint8_t use_delay;
	float delay_timer;
} ncx_sound_t;

void ncx_sound_init(void);
void ncx_sound_terminate(void);

ncx_sound_t ncx_sound_create(const char *paths, uint8_t sample_count,
		uint8_t use_delay);
void ncx_sound_destroy(ncx_sound_t *ncx_sound);
void ncx_sound_play(ncx_sound_t sound, float gain, float pitch,
		struct ncx_vec3 pos, uint8_t looping, uint8_t index);
void ncx_sound_play_delay(ncx_sound_t *sound, float gain, float pitch,
		struct ncx_vec3 pos, uint8_t index, float time_delta);
void ncx_sound_stop(ncx_sound_t sound);
void ncx_sound_set_float(ncx_sound_t sound, uint32_t prop, float f);

#endif

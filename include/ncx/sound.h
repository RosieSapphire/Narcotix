#ifndef __NCX_SOUND_H_
#define __NCX_SOUND_H_

#include "ncx/vec3.h"

enum ncx_sound_props {
	SP_GAIN = 0x100A,
	SP_PITCH = 0x1003,
};

struct ncx_sound {
	uint32_t source;
	uint32_t *delay_sources;
	uint32_t *buffers;
	uint8_t buffer_count;
	uint8_t use_delay;
	float delay_timer;
};

/*
 * Initializes OpenAL for using audio.
 */
void ncx_sound_init(void);

/*
 * Terminates the OpenAL context; audio go bye-bye :3
 */
void ncx_sound_terminate(void);

struct ncx_sound ncx_sound_create(const char *paths,
		uint8_t sample_count, uint8_t use_delay);

void ncx_sound_destroy(struct ncx_sound *ncx_sound);

void ncx_sound_play(struct ncx_sound sound, float gain, float pitch,
		struct ncx_vec3 pos, uint8_t looping, uint8_t index);

void ncx_sound_play_delay(struct ncx_sound *sound, float gain, float pitch,
		struct ncx_vec3 pos, uint8_t index, float time_delta);

void ncx_sound_stop(struct ncx_sound sound);
void ncx_sound_set_float(struct ncx_sound sound, uint32_t prop, float f);

#endif

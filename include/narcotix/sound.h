#ifndef __NCX_SOUND_H_
#define __NCX_SOUND_H_

#include <stdint.h>

typedef struct {
	uint32_t source;
	uint32_t *delay_sources;
	uint32_t *buffers;
	uint8_t buffer_count;
	uint8_t use_delay;
	float delay_timer;
} ncx_sound_t;

ncx_sound_t ncx_sound_create(const char *paths,
		const uint8_t sample_count, const uint8_t use_delay);
void ncx_sound_play(const ncx_sound_t ncx_sound,
		const float gain, const float pitch, const float *pos,
		const uint8_t looping, const uint8_t index);
void ncx_sound_play_delay(ncx_sound_t *ncx_sound, const float gain,
		const float pitch, const float *pos, const uint8_t index,
		const float time_delta);
void ncx_sound_destroy(ncx_sound_t *ncx_sound);

#endif

#ifndef NCX_SOUND_H
#define NCX_SOUND_H

#include <stdint.h>

typedef struct {
	uint32_t source;
	uint32_t padding0;
	uint32_t *delay_sources;
	uint32_t *buffers;
	uint8_t buffer_count;
	uint8_t use_delay;
	uint16_t padding1;
	float delay_timer;
} NCXSound;

NCXSound ncx_sound_create_internal(const char *paths, const uint8_t sample_count, const uint8_t use_delay, const char *file, const uint32_t line);
#define ncx_sound_create(PATHS, SAMPLE_COUNT, USE_DELAY) ncx_sound_create_internal(PATHS, SAMPLE_COUNT, USE_DELAY, __FILE__, __LINE__)

void ncx_sound_play(const NCXSound ncx_sound, const float gain, const float pitch, const float *pos, const uint8_t looping, const uint8_t index);
void ncx_sound_play_delay(NCXSound *ncx_sound, const float gain, const float pitch, const float *pos, const uint8_t index, const float time_delta);
void ncx_sound_destroy(NCXSound *ncx_sound);

#endif

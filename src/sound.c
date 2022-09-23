#include "narcotix/sound.h"

#include "narcotix/cglm/cglm.h"
#include <AL/al.h>
#include <sndfile.h>
#include <limits.h>

void ncx_sound_create(ncx_sound_t *ncx_sound, const char *paths, const uint8_t sample_count, const uint8_t use_delay) {
	uint32_t path_length;
	const int32_t formats[2] = {
		AL_FORMAT_MONO16,
		AL_FORMAT_STEREO16,
	};

	ncx_sound->buffer_count = sample_count;
	ncx_sound->buffers = calloc(sample_count, sizeof(uint32_t));
	ncx_sound->delay_timer = 500.0f;

	alGenSources(1, &ncx_sound->source);
	alSourcefv(ncx_sound->source, AL_VELOCITY, GLM_VEC3_ZERO);

	ncx_sound->delay_sources = NULL;
	ncx_sound->use_delay = use_delay;
	if(use_delay) {
		ncx_sound->delay_sources = calloc(8, sizeof(uint32_t));
		alGenSources(8, ncx_sound->delay_sources);
	}

	{
		const char *c = paths;
		while(*c) {
			c++; 
		}
		path_length = (uint32_t)(c - paths) + 1;
	}
	
	for(uint8_t i = 0; i < sample_count; i++) {
		SNDFILE *file;
		SF_INFO file_info;
		uint64_t frame_count;
		int32_t format;
		int16_t *data;
		uint64_t size;

		file = sf_open(paths + (i * path_length), SFM_READ, &file_info);
		format = formats[file_info.channels - 1];
		#ifdef DEBUG
			if(!file) {
				printf("ERROR: Sound loading fucked up: %s\n", paths + (i * path_length));
				return;
			}

			if(file_info.frames < 1 || file_info.frames > (sf_count_t)(INT_MAX / sizeof(int16_t)) / file_info.channels) {
				sf_close(file);
				printf("ERROR: Sample rate fucked up\n");
				return;
			}

			if(!format) {
				sf_close(file);
				printf("ERROR: Sound format fucked up\n");
				return;
			}
		#endif

		data = malloc((size_t)(file_info.frames * file_info.channels) * sizeof(int16_t));
		frame_count = (size_t)sf_readf_short(file, data, file_info.frames);
		#ifdef DEBUG
			if(frame_count < 1) {
				free(data);
				sf_close(file);
				printf("ERROR: Sound frame count fucked up\n");
				return;
			}
		#endif

		size = frame_count * (uint64_t)file_info.channels * sizeof(int16_t);

		alGenBuffers(1, &ncx_sound->buffers[i]);
		alBufferData(ncx_sound->buffers[i], format, (const void *)data, (int32_t)size, file_info.samplerate);

		#ifdef DEBUG
		{
			if(alGetError()) {
				if(ncx_sound->buffers[i] && alIsBuffer(ncx_sound->buffers[i])) {
					alDeleteBuffers(1, &ncx_sound->buffers[i]);
				}
			}
		}
		#endif
		free(data);
		sf_close(file);
	}
	
	alSourcei(ncx_sound->source, AL_BUFFER, (int32_t)ncx_sound->buffers[0]);
}

void ncx_sound_play(const ncx_sound_t ncx_sound, const float gain, const float pitch, const float *pos, const uint8_t looping, const uint8_t index) {
	alSourceStop(ncx_sound.source);
	alSourcef(ncx_sound.source, AL_GAIN, gain);
	alSourcef(ncx_sound.source, AL_PITCH, pitch);
	alSourcefv(ncx_sound.source, AL_POSITION, pos);
	alSourcei(ncx_sound.source, AL_LOOPING, looping);
	alSourcei(ncx_sound.source, AL_BUFFER, (int32_t)ncx_sound.buffers[index]);
	alSourcePlay(ncx_sound.source);
}

void ncx_sound_play_delay(ncx_sound_t *ncx_sound, const float gain, const float pitch, const float *pos, const uint8_t index, const float time_delta) {
	const float delay_timer_last = ncx_sound->delay_timer;
	float times[8];
	float gains[8];
	ncx_sound->delay_timer += time_delta;
	for(uint8_t i = 0; i < 8; i++) {
		times[i] = ((8.0f - (i + 1)) / 8);
		gains[i] = (1.0f - (times[i])) * gain * gain;
		alSourcei(ncx_sound->delay_sources[i], AL_BUFFER, (int32_t)ncx_sound->buffers[index]);
		alSourcef(ncx_sound->delay_sources[i], AL_GAIN, gains[i]);
		alSourcef(ncx_sound->delay_sources[i], AL_PITCH, pitch + pitch / i);
		alSourcefv(ncx_sound->delay_sources[i], AL_POSITION, pos);
		alSourcei(ncx_sound->delay_sources[i], AL_LOOPING, 0);
		if(ncx_sound->delay_timer >= times[i] && delay_timer_last < times[i]) {
			alSourcePlay(ncx_sound->delay_sources[i]);
			// printf("%i: %f\n", ncx_sound->delay_sources[i], (double)(gains[i]));
		}
	}
}

void ncx_sound_destroy(ncx_sound_t *ncx_sound) {
	alDeleteBuffers(ncx_sound->buffer_count, ncx_sound->buffers);
	free(ncx_sound->buffers);

	if(ncx_sound->use_delay) {
		alDeleteSources(8, ncx_sound->delay_sources);
		free(ncx_sound->delay_sources);
	}
}

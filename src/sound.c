#include "narcotix/sound.h"

#include "narcotix/cglm/cglm.h"
#include <AL/al.h>
#include <sndfile.h>
#include <limits.h>

#ifdef DEBUG
	#include "narcotix/debug.h"
#endif

NCXSound ncx_sound_create_internal(const char *paths, const uint8_t sample_count, const uint8_t use_delay, const char *file, const uint32_t line) {
	NCXSound sound;
	uint32_t path_length;
	const int32_t formats[2] = {
		AL_FORMAT_MONO16,
		AL_FORMAT_STEREO16,
	};

	sound.buffer_count = sample_count;
	sound.buffers = calloc(sample_count, sizeof(uint32_t));
	sound.delay_timer = 500.0f;

	alGenSources(1, &sound.source);
	alSourcefv(sound.source, AL_VELOCITY, GLM_VEC3_ZERO);

	sound.delay_sources = NULL;
	sound.use_delay = use_delay;
	if(use_delay) {
		sound.delay_sources = calloc(8, sizeof(uint32_t));
		alGenSources(8, sound.delay_sources);
	}

	{
		const char *c = paths;
		while(*c) {
			c++; 
		}
		path_length = (uint32_t)(c - paths) + 1;
	}
	
	for(uint8_t i = 0; i < sample_count; i++) {
		const char *current_path = paths + (i * path_length);
		SNDFILE *snd_file;
		SF_INFO file_info;
		uint64_t frame_count;
		int32_t format;
		int16_t *data;
		uint64_t size;

		snd_file = sf_open(current_path, SFM_READ, &file_info);
		#ifdef DEBUG
		{
			if(!snd_file) {
				fprintf(stderr, "%sNARCOTIX::SOUND::ERROR: %sLoading sound from path: %s'%s'%s fucked up. %s(Caused at '%s' line %u).\n", D_COLOR_RED, D_COLOR_YELLOW, D_COLOR_GREEN, current_path, D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
				return sound;
			}

			if(file_info.frames < 1 || file_info.frames > (sf_count_t)(INT_MAX / sizeof(int16_t)) / file_info.channels) {
				sf_close(snd_file);
				fprintf(stderr, "%sNARCOTIX::SOUND::ERROR: %sSound loaded from %s'%s'%s has a fucked up sample rate. %s(Caused at '%s' line %u)\n", D_COLOR_RED, D_COLOR_YELLOW, D_COLOR_GREEN, current_path, D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
				return sound;
			}
		}
		#endif
		format = formats[file_info.channels - 1];
		#ifdef DEBUG
			if(!format) {
				sf_close(snd_file);
				fprintf(stderr, "%sNARCOTIX::SOUND::ERROR: %sSound loaded from %s'%s'%s has an invalid format/channel count. %s(Caused at '%s' line '%u)\n",
						D_COLOR_RED, D_COLOR_YELLOW, D_COLOR_GREEN, current_path, D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
				return sound;
			}
		#endif

		data = malloc((size_t)(file_info.frames * file_info.channels) * sizeof(int16_t));
		frame_count = (size_t)sf_readf_short(snd_file, data, file_info.frames);
		#ifdef DEBUG
			if(frame_count < 1) {
				free(data);
				sf_close(snd_file);
				fprintf(stderr, "%sNARCOTIX::SOUND::ERROR: %sLoaded sound from %s'%s'%s seems to have an invalid length. %s(Caused at '%s' line '%u'\n",
						D_COLOR_RED, D_COLOR_YELLOW, D_COLOR_GREEN, current_path, D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
				return sound;
			}
		#endif

		size = frame_count * (uint64_t)file_info.channels * sizeof(int16_t);

		alGenBuffers(1, &sound.buffers[i]);
		alBufferData(sound.buffers[i], format, (const void *)data, (int32_t)size, file_info.samplerate);

		#ifdef DEBUG
		{
			if(alGetError()) {
				if(sound.buffers[i] && alIsBuffer(sound.buffers[i])) {
					alDeleteBuffers(1, &sound.buffers[i]);
				}
			}
		}
		#endif
		free(data);
		sf_close(snd_file);
		printf("%sNARCOTIX::SOUND::CREATE: %sSound from %s'%s'%s was imported successfully. %s(Caused at '%s' line '%u'\n", D_COLOR_GREEN, D_COLOR_YELLOW, D_COLOR_GREEN, current_path, D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
	}
	
	alSourcei(sound.source, AL_BUFFER, (int32_t)sound.buffers[0]);
	return sound;
}

void ncx_sound_play(const NCXSound sound, const float gain, const float pitch, const float *pos, const uint8_t looping, const uint8_t index) {
	alSourceStop(sound.source);
	alSourcef(sound.source, AL_GAIN, gain);
	alSourcef(sound.source, AL_PITCH, pitch);
	alSourcefv(sound.source, AL_POSITION, pos);
	alSourcei(sound.source, AL_LOOPING, looping);
	alSourcei(sound.source, AL_BUFFER, (int32_t)sound.buffers[index]);
	alSourcePlay(sound.source);
}

void ncx_sound_play_delay(NCXSound *sound, const float gain, const float pitch, const float *pos, const uint8_t index, const float time_delta) {
	const float delay_timer_last = sound->delay_timer;
	float times[8];
	float gains[8];
	sound->delay_timer += time_delta;
	for(uint8_t i = 0; i < 8; i++) {
		times[i] = ((8.0f - (i + 1)) / 8);
		gains[i] = (1.0f - (times[i])) * gain * gain;
		alSourcei(sound->delay_sources[i], AL_BUFFER, (int32_t)sound->buffers[index]);
		alSourcef(sound->delay_sources[i], AL_GAIN, gains[i]);
		alSourcef(sound->delay_sources[i], AL_PITCH, pitch + pitch / i);
		alSourcefv(sound->delay_sources[i], AL_POSITION, pos);
		alSourcei(sound->delay_sources[i], AL_LOOPING, 0);
		if(sound->delay_timer >= times[i] && delay_timer_last < times[i]) {
			alSourcePlay(sound->delay_sources[i]);
			// printf("%i: %f\n", sound->delay_sources[i], (double)(gains[i]));
		}
	}
}

void ncx_sound_destroy(NCXSound *sound) {
	alDeleteBuffers(sound->buffer_count, sound->buffers);
	free(sound->buffers);

	if(sound->use_delay) {
		alDeleteSources(8, sound->delay_sources);
		free(sound->delay_sources);
	}
}

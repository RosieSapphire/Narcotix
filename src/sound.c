#include "ncx/sound.h"
#include "ncx/vec3.h"

#include <AL/al.h>
#include <AL/alc.h>
#include <malloc.h>
#include <sndfile.h>
#include <assert.h>

static ALCdevice *sound_device;
static ALCcontext *sound_context;

void ncx_sound_init(void)
{
	sound_device = alcOpenDevice(NULL);
	if(!sound_device) {
		fprintf(stderr, "OPENAL ERROR: Failed to open device.\n");
		assert(0);
	}
	
	sound_context = alcCreateContext(sound_device, NULL);
	if(!sound_context) {
		fprintf(stderr, "OPENAL ERROR: Failed to create context.\n");
		assert(0);
	}
	alcMakeContextCurrent(sound_context);
}

void ncx_sound_terminate(void)
{
	alcDestroyContext(sound_context);
	alcCloseDevice(sound_device);
}

struct ncx_sound ncx_sound_create(const char *paths, uint8_t smpl_count,
		uint8_t use_delay)
{
	uint32_t path_length;
	int32_t formats[2] = {
		AL_FORMAT_MONO16,
		AL_FORMAT_STEREO16,
	};

	struct ncx_sound sound;
	sound.buffer_count = smpl_count;
	sound.buffers = malloc(smpl_count * sizeof(uint32_t));
	sound.delay_timer = 500.0f;

	alGenSources(1, &sound.source);
	alSource3f(sound.source, AL_VELOCITY, 0, 0, 0);

	sound.delay_sources = NULL;
	sound.use_delay = use_delay;
	if(use_delay) {
		sound.delay_sources = malloc(8 * sizeof(uint32_t));
		alGenSources(8, sound.delay_sources);
	}

	{
		const char *c = paths;
		while(*c) {
			c++; 
		}
		path_length = (uint32_t)(c - paths) + 1;
	}
	
	for(uint8_t i = 0; i < smpl_count; i++) {
		const char *current_path = paths + (i * path_length);
		SNDFILE *snd_file;
		SF_INFO file_info;
		uint64_t frame_count;
		int32_t format;
		int16_t *data;
		uint64_t size;

		snd_file = sf_open(current_path, SFM_READ, &file_info);
		if(!snd_file) {
			fprintf(stderr, "SOUND ERROR: Couldn't "
					"load sound from '%s'.\n",
					current_path);
			assert(0);
		}

		format = formats[file_info.channels - 1];
		if(!format) {
			fprintf(stderr, "SOUND ERROR: Sound from"
					" '%s' doesn't have proper format.\n",
					current_path);
			assert(0);
		}

		data = malloc((size_t)(file_info.frames * file_info.channels) *
				sizeof(int16_t));
		assert(data);

		frame_count =
			(size_t)sf_readf_short(snd_file,
					data, file_info.frames);
		assert(frame_count);

		size = frame_count * file_info.channels * sizeof(int16_t);

		alGenBuffers(1, &sound.buffers[i]);
		alBufferData(sound.buffers[i], format, (void *)data,
				(int32_t)size, file_info.samplerate);

		assert(!alGetError());
		free(data);
		sf_close(snd_file);
	}
	
	alSourcei(sound.source, AL_BUFFER, (int32_t)sound.buffers[0]);
	return sound;
}

void ncx_sound_play(struct ncx_sound sound, float gain, float pitch,
		struct ncx_vec3 pos, uint8_t looping, uint8_t index)
{
	alSourceStop(sound.source);
	alSourcef(sound.source, AL_GAIN, gain);
	alSourcef(sound.source, AL_PITCH, pitch);
	alSource3f(sound.source, AL_POSITION, pos.x, pos.y, pos.z);
	alSourcei(sound.source, AL_LOOPING, looping);
	alSourcei(sound.source, AL_BUFFER, (int32_t)sound.buffers[index]);
	alSourcePlay(sound.source);
}

void ncx_sound_play_delay(struct ncx_sound *sound, float gain, float pitch,
		struct ncx_vec3 pos, uint8_t index, float time_delta)
{
	float delay_last = sound->delay_timer;
	float times[8];
	float gains[8];
	sound->delay_timer += time_delta;
	for(uint8_t i = 0; i < 8; i++) {
		times[i] = ((8.0f - (i + 1)) / 8);
		gains[i] = (1.0f - (times[i])) * gain * gain;
		alSourcei(sound->delay_sources[i], AL_BUFFER,
				(int32_t)sound->buffers[index]);
		alSourcef(sound->delay_sources[i], AL_GAIN, gains[i]);
		alSourcef(sound->delay_sources[i], AL_PITCH, pitch + pitch / i);
		alSource3f(sound->delay_sources[i], AL_POSITION,
				pos.x, pos.y, pos.z);
		alSourcei(sound->delay_sources[i], AL_LOOPING, 0);

		if(sound->delay_timer < times[i] || delay_last >= times[i])
			continue;

		alSourcePlay(sound->delay_sources[i]);
	}
}

void ncx_sound_stop(struct ncx_sound sound)
{
	alSourceStop(sound.source);
}

void ncx_sound_destroy(struct ncx_sound *sound)
{
	alDeleteBuffers(sound->buffer_count, sound->buffers);
	free(sound->buffers);

	if(sound->use_delay) {
		alDeleteSources(8, sound->delay_sources);
		free(sound->delay_sources);
	}
}

void ncx_sound_set_float(struct ncx_sound sound, uint32_t prop, float f)
{
	alSourcef(sound.source, prop, f);
}

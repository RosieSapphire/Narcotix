#include "narcotix/sound_engine.h"
#include <AL/alc.h>
#include <assert.h>

#define NULL ((void *)0)

static ALCdevice *sound_device;
static ALCcontext *sound_context;

void ncx_sound_engine_init(void) {
	sound_device = alcOpenDevice(NULL);
	assert(sound_device);
	
	sound_context = alcCreateContext(sound_device, NULL);
	assert(sound_context);
	alcMakeContextCurrent(sound_context);
}

void ncx_sound_engine_terminate(void) {
	alcDestroyContext(sound_context);
	alcCloseDevice(sound_device);
}

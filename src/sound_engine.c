#include "narcotix/sound_engine.h"

#include <AL/alc.h>
#include <assert.h>

#define NULL ((void *)0)

NCXSoundEngine ncx_sound_engine_create(void) {
	NCXSoundEngine engine;

	engine.sound_device = alcOpenDevice(NULL);
	assert(engine.sound_device);
	
	engine.sound_context = alcCreateContext(engine.sound_device, NULL);
	assert(engine.sound_context);
	alcMakeContextCurrent(engine.sound_context);

	return engine;
}

void ncx_sound_engine_destroy(const NCXSoundEngine engine) {
	alcDestroyContext(engine.sound_context);
	alcCloseDevice(engine.sound_device);
}

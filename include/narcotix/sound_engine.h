#ifndef NCX_SOUND_ENGINE_H
#define NCX_SOUND_ENGINE_H

#include <AL/alc.h>

typedef struct {
	ALCdevice *sound_device;
	ALCcontext *sound_context;
} NCXSoundEngine;

NCXSoundEngine ncx_sound_engine_create();
void ncx_sound_engine_destroy(const NCXSoundEngine engine);

#endif

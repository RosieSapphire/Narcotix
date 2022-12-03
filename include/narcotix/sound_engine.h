#ifndef __NCX_SOUND_ENGINE_H_
#define __NCX_SOUND_ENGINE_H_

#include <AL/alc.h>

typedef struct {
	ALCdevice *sound_device;
	ALCcontext *sound_context;
} NCXSoundEngine;

NCXSoundEngine ncx_sound_engine_create(void);
void ncx_sound_engine_destroy(const NCXSoundEngine engine);

#endif

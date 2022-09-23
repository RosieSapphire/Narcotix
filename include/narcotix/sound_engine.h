#ifndef NCX_SOUND_ENGINE_H
#define NCX_SOUND_ENGINE_H

#include <AL/alc.h>

typedef struct {
	ALCdevice *sound_device;
	ALCcontext *sound_context;
} ncx_sound_engine_t;

void ncx_sound_engine_create(ncx_sound_engine_t *engine);

#endif

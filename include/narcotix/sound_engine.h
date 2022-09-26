#ifndef NCX_SOUND_ENGINE_H
#define NCX_SOUND_ENGINE_H

#include <AL/alc.h>
#include <stdint.h>

typedef struct {
	ALCdevice *sound_device;
	ALCcontext *sound_context;
} NCXSoundEngine;

NCXSoundEngine ncx_sound_engine_create_internal(const char *file, const uint32_t line);
#define ncx_sound_engine_create() ncx_sound_engine_create_internal(__FILE__, __LINE__)
void ncx_sound_engine_destroy(const NCXSoundEngine engine);

#endif

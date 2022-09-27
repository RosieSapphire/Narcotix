#include "narcotix/sound_engine.h"

#include <stdio.h>
#include <AL/al.h>
#include <AL/alc.h>

#ifdef DEBUG
#include "narcotix/debug.h"
#endif

NCXSoundEngine ncx_sound_engine_create_internal(const char *file, const uint32_t line) {
	NCXSoundEngine engine;
	#ifdef DEBUG
	    const char *sound_device_name;
	#endif
	engine.sound_device = alcOpenDevice(NULL);
	#ifdef DEBUG
	    if(!engine.sound_device) {
	        fprintf(stderr, "%sNARCOTIX::SOUND::ENGINE::ERROR: %sOpenAL failed to load the current audio device. %s(Caused at '%s' line %u)\n", D_COLOR_RED, D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
	        return engine;
	    }
	#endif
	
	engine.sound_context = alcCreateContext(engine.sound_device, NULL);
	#ifdef DEBUG
	    if(!engine.sound_context) {
	        fprintf(stderr, "%sNARCOTIX::SOUND::ENGINE::ERROR: %sOpenAL fucked up creating an audio context. %s(Caused at '%s' line %u)\n", D_COLOR_RED, D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
	        return engine;
	    }
	
		if(!alcMakeContextCurrent(engine.sound_context)) {
	        fprintf(stderr, "%sNARCOTIX::SOUND::ENGINE::ERROR: %sOpenAL failed to make context current. %s(Caused at '%s' line %u)\n", D_COLOR_RED, D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
		    return engine;
		}

	    sound_device_name = NULL;
	    if(alcIsExtensionPresent(engine.sound_device, "ALC_ENUMERATE_ALL_EXT")) {
	        sound_device_name = alcGetString(engine.sound_device, ALC_ALL_DEVICES_SPECIFIER);
	    } else {
	    	fprintf(stderr, "%sNARCOTIX::SOUND::ENGINE::ERROR: %sFucked up finding name of current audio device. %s(Caused at '%s' line %u)\n", D_COLOR_RED, D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
			return engine;
		}
	    
	    if(!sound_device_name || alcGetError(engine.sound_device) != AL_NO_ERROR) {
	        sound_device_name = alcGetString(engine.sound_device, ALC_DEVICE_SPECIFIER);
	    }
	#else
		alcMakeContextCurrent(engine.sound_context);
	#endif

	#ifdef DEBUG
		printf("%sNARCOTIX::SOUND::ENGINE::CREATE: %sOpenAL has successfully created a context and is running on device: %s'%s'%s. %s(Caused at '%s' line %u)\n",
				D_COLOR_GREEN, D_COLOR_YELLOW, D_COLOR_GREEN, sound_device_name, D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
	#endif
	return engine;
}

void ncx_sound_engine_destroy(const NCXSoundEngine engine) {
	alcDestroyContext(engine.sound_context);
	alcCloseDevice(engine.sound_device);
}

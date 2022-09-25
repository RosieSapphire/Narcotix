#include "narcotix/sound_engine.h"

#include <stdio.h>
#include <AL/al.h>
#include <AL/alc.h>

NCXSoundEngine ncx_sound_engine_create() {
	NCXSoundEngine engine;
	#ifdef DEBUG
	    const char *sound_device_name;
	#endif
	engine.sound_device = alcOpenDevice(NULL);
	#ifdef DEBUG
	    if(!engine.sound_device) {
	        printf("ERROR: Audio Device fucked up.");
	        return engine;
	    }
	#endif
	
	engine.sound_context = alcCreateContext(engine.sound_device, NULL);
	#ifdef DEBUG
	    if(!engine.sound_context) {
	        printf("ERROR: Audio Context fucked up.");
	        return engine;
	    }
	
		if(!alcMakeContextCurrent(engine.sound_context)) {
		    printf("ERROR: Making context fucked up.");
		    return engine;
		}

	    sound_device_name = NULL;
	    if(alcIsExtensionPresent(engine.sound_device, "ALC_ENUMERATE_ALL_EXT")) {
	        sound_device_name = alcGetString(engine.sound_device, ALC_ALL_DEVICES_SPECIFIER);
	    }
	    
	    if(!sound_device_name || alcGetError(engine.sound_device) != AL_NO_ERROR) {
	        sound_device_name = alcGetString(engine.sound_device, ALC_DEVICE_SPECIFIER);
	    }
	    printf("SOUND DEVICE: %s\n", sound_device_name);
	#else
		alcMakeContextCurrent(engine.sound_context);
	#endif

	return engine;
}

void ncx_sound_engine_destroy(const NCXSoundEngine engine) {
	alcDestroyContext(engine.sound_context);
	alcCloseDevice(engine.sound_device);
}

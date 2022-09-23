#include "narcotix/file.h"

#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>

#ifdef DEBUG
	#include "narcotix/debug.h"
#endif

char *ncx_file_load_contents(const char *path) {
	uint32_t size;
	FILE *file;
	char *buffer;

	file = fopen(path, "rb");
	#ifdef DEBUG
    	if(!file) {
    	    fprintf(stderr, "%sNARCOTIX::FILE::ERROR:\t\t%sLoading from file %s'%s'%s fucked up. %s(Caused at '%s' line %i)\n", D_COLOR_RED, D_COLOR_YELLOW, D_COLOR_GREEN, path, D_COLOR_YELLOW, D_COLOR_DEFAULT, __FILE__, (__LINE__));
    	    return NULL;
    	}
	#endif

    fseek(file, 0L, SEEK_END);
    size = (uint32_t)ftell(file);
    rewind(file);

    buffer = malloc(size * sizeof(GLchar));
	fread(buffer, sizeof(char), size, file);
	buffer[size - 1] = 0;

    fclose(file);

	return buffer;
}

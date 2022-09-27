#include "narcotix/file.h"

#include <stdio.h>
#include <stdlib.h>

#include <GLFW/glfw3.h>

#ifdef DEBUG
	#include "narcotix/debug.h"
#endif

char *ncx_file_load_contents_internal(const char *path, const char *file, const uint32_t line) {
	uint32_t size;
	FILE *file_struct;
	char *buffer;

	file_struct = fopen(path, "rb");
	#ifdef DEBUG
    	if(!file_struct) {
    	    fprintf(stderr, "%sNARCOTIX::FILE::ERROR: %sLoading from file %s'%s'%s fucked up. %s(Caused at '%s' line %i)\n", D_COLOR_RED, D_COLOR_YELLOW, D_COLOR_GREEN, path, D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
    	    return NULL;
    	}
	#endif

    fseek(file_struct, 0L, SEEK_END);
    size = (uint32_t)ftell(file_struct);
    rewind(file_struct);

    buffer = malloc(size * sizeof(GLchar));
	fread(buffer, sizeof(char), size, file_struct);
	buffer[size - 1] = 0;

    fclose(file_struct);

	return buffer;
}

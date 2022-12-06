#include "narcotix/file.h"

#include <stdio.h>
#include <assert.h>
#include <malloc.h>

char *ncx_file_load(const char *path) {
	size_t size;
	FILE *file;
	char *buffer;

	file = fopen(path, "rb");
	if(!file) {
		fprintf(stderr, "FILE ERROR: Failed to load file from path '%s'.\n",
				path);
		assert(0);
	}

    fseek(file, 0L, SEEK_END);
    size = (uint32_t)ftell(file);
    rewind(file);

    buffer = malloc(size * sizeof(char));
	fread(buffer, sizeof(char), size, file);
	buffer[size - 1] = 0;

    fclose(file);

	return buffer;
}

void ncx_file_unload(char *buffer) {
	free(buffer);
}

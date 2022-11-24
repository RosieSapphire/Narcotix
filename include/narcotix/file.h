#ifndef NCX_FILE_H
#define NCX_FILE_H

#include <stdint.h>

char *ncx_file_load_contents_internal(const char *path, const char *file,
		const uint32_t line);
void ncx_file_unload_contents(char *buffer);
#define ncx_file_load_contents(PATH) \
	ncx_file_load_contents_internal(PATH, __FILE__, __LINE__)

#endif

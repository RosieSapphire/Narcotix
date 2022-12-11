#ifndef __NCX_FILE_H_
#define __NCX_FILE_H_

#include <stdint.h>

char *ncx_file_load(const char *path);
void ncx_file_unload(char *buffer);

#endif

#ifndef __NCX_TEXTURE_H_
#define __NCX_TEXTURE_H_

#include <stdint.h>

#define T_LINEAR GL_LINEAR
#define T_LINEAR_MIP GL_LINEAR_MIPMAP_LINEAR
#define T_REPEAT_FLIP GL_MIRRORED_REPEAT
#define T_CLAMP GL_CLAMP_TO_EDGE

typedef uint32_t ncx_texture_t;

ncx_texture_t ncx_texture_create(const char *path,
		const int32_t wrap_mode, const int32_t min_filter,
		const int32_t mag_filter, const uint8_t mipmap);
void ncx_textures_destroy(ncx_texture_t *start, const uint8_t texture_count);

void ncx_texture_use(ncx_texture_t tex, const uint32_t slot);

#endif

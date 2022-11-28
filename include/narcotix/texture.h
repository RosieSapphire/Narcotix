#ifndef NCX_TEXTURE_H
#define NCX_TEXTURE_H

#include <stdint.h>

typedef uint32_t NCXTexture;

NCXTexture ncx_texture_create_internal(const char *path,
		const int32_t wrap_mode, const int32_t min_filter,
		const int32_t mag_filter, const uint8_t mipmap,
		const char *file, const uint32_t line);

#define ncx_texture_create(PATH, WRAP_MODE, MIN_FILTER, MAG_FILTER, MIPMAP) \
	ncx_texture_create_internal(PATH, WRAP_MODE, MIN_FILTER, MAG_FILTER, \
			MIPMAP, __FILE__, __LINE__)

void ncx_textures_destroy(NCXTexture *start, const uint8_t texture_count);

void ncx_texture_use(NCXTexture tex, const uint32_t slot);

#endif

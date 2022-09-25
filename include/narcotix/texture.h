#ifndef NCX_TEXTURE_H
#define NCX_TEXTURE_H

#include <stdint.h>

typedef uint32_t NCXTexture;
NCXTexture ncx_texture_create(const char *path, const int32_t wrap_mode, const int32_t min_filter, const int32_t mag_filter, const uint8_t mipmap);
void ncx_textures_destroy(NCXTexture *start, const uint8_t texture_count);

#endif

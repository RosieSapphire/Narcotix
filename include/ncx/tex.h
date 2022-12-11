#ifndef __NCX_TEX_H_
#define __NCX_TEX_H_

#include <stdint.h>

#define T_LINEAR GL_LINEAR
#define T_LINEAR_MIP GL_LINEAR_MIPMAP_LINEAR
#define T_REPEAT_FLIP GL_MIRRORED_REPEAT
#define T_CLAMP GL_CLAMP_TO_EDGE

typedef uint32_t ncx_tex_t;

ncx_tex_t ncx_tex_create(const char *path,
		const int32_t wrap_mode, const int32_t min_filter,
		const int32_t mag_filter, const uint8_t mipmap);

void ncx_tex_destroy(ncx_tex_t *start, const uint8_t tex_count);
void ncx_tex_use(ncx_tex_t tex, const uint32_t slot);

#endif

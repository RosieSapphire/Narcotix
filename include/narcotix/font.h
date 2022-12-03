#ifndef __NCX_FONT_H_
#define __NCX_FONT_H_

#include "narcotix/font_internal.h"

#define ncx_font_shader_create(FONT_PATH_VERT, FONT_PATH_FRAG) \
	ncx_font_shader_create_internal(FONT_PATH_VERT, FONT_PATH_FRAG, \
			__FILE__, __LINE__)
#define ncx_font_create(P) ncx_font_create_internal(P, __FILE__, __LINE__)
#define ncx_font_destroy(FONT) \
	ncx_font_destroy_internal(FONT, __FILE__, __LINE__)

void ncx_font_draw(const NCXFont font, const char *string, float *pos,
		const float *color, const float scale, float *window_size,
		const NCXShader shader);

#endif

#ifndef __NCX_FONT_INTERNAL_H_
#define __NCX_FONT_INTERNAL_H_

#include "narcotix/shader.h"

typedef struct {
	uint32_t texture;
	int32_t size[2];
	int32_t bearing[2];
	uint32_t advance;
} NCXCharacter;

typedef struct {
	uint32_t vao;
	uint32_t vbo;
	NCXCharacter *characters;
} NCXFont;

NCXShader ncx_font_shader_create_internal(const char *font_path_vert,
		const char *font_path_frag, const char *file, const uint32_t line);
NCXFont ncx_font_create_internal(const char *path, const char *file,
		const uint32_t line);
void ncx_font_destroy_internal(NCXFont *font, const char *file,
		const uint32_t line);

#endif

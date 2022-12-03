#ifndef __NCX_FONT_H_
#define __NCX_FONT_H_

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

NCXShader ncx_font_shader_create(const char *font_path_vert,
		const char *font_path_frag);
NCXFont ncx_font_create(const char *path);
void ncx_font_destroy(NCXFont *font);

void ncx_font_draw(const NCXFont font, const char *string, float *pos,
		const float *color, const float scale, float *window_size,
		const NCXShader shader);

#endif

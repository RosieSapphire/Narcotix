#ifndef __NCX_FONT_H_
#define __NCX_FONT_H_

#include "narcotix/shader.h"

typedef struct {
	uint32_t texture;
	int32_t size[2];
	int32_t bearing[2];
	uint32_t advance;
} ncx_character_t;

typedef struct {
	uint32_t vao;
	uint32_t vbo;
	ncx_character_t *characters;
} ncx_font_t;

ncx_shader_t ncx_font_shader_create(const char *font_path_vert,
		const char *font_path_frag);
ncx_font_t ncx_font_create(const char *path);
void ncx_font_destroy(ncx_font_t *font);

void ncx_font_draw(const ncx_font_t font, const char *string, float *pos,
		const float *color, const float scale, float *window_size,
		const ncx_shader_t shader);

#endif

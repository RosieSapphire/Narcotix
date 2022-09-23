#ifndef NCX_FONT_H
#define NCX_FONT_H

#include <stdint.h>

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

void ncx_font_shader_create(const char *font_path_vert, const char *font_path_frag);
ncx_font_t ncx_font_create(const char *path);
void ncx_font_draw(const ncx_font_t ncx_font, const char *string, float *pos, const float *color, const float scale, float *window_size);
void ncx_font_destroy(ncx_font_t *ncx_font);
void ncx_font_shader_destroy(void);

#endif

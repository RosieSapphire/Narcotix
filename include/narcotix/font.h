#ifndef __NCX_FONT_H_
#define __NCX_FONT_H_

#include <stdint.h>

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

void ncx_font_shader_create_internal(const char *font_path_vert,
		const char *font_path_frag, const char *file, const uint32_t line);
#define ncx_font_shader_create(FONT_PATH_VERT, FONT_PATH_FRAG) \
	ncx_font_shader_create_internal(FONT_PATH_VERT, FONT_PATH_FRAG, \
			__FILE__, __LINE__)

NCXFont ncx_font_create_internal(const char *path, const char *file,
		const uint32_t line);
#define ncx_font_create(P) ncx_font_create_internal(P, __FILE__, __LINE__)

void ncx_font_draw(const NCXFont font, const char *string, float *pos,
		const float *color, const float scale, float *window_size);
void ncx_font_destroy(NCXFont *font);

void ncx_font_shader_destroy(void);

#endif

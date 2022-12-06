#ifndef __NCX_FONT_H_
#define __NCX_FONT_H_

#include "narcotix/shader.h"
#include "narcotix/ivec2.h"
#include "narcotix/vec2.h"

typedef struct {
	uint32_t texture;
	ncx_ivec2_t size;
	ncx_ivec2_t bearing;
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

void ncx_font_draw(ncx_font_t font, const char *string, ncx_vec2_t pos,
		ncx_vec3_t color, float scale, ncx_vec2_t window_size,
		ncx_shader_t shader);

#endif

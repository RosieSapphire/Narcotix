#ifndef __NCX_FONT_H_
#define __NCX_FONT_H_

#include "ncx/shader.h"
#include "ncx/ivec2.h"
#include "ncx/vec2.h"

struct ncx_char {
	uint32_t texture;
	struct ncx_ivec2 size;
	struct ncx_ivec2 bearing;
	uint32_t advance;
};

struct ncx_font {
	uint32_t vao;
	uint32_t vbo;

	/* TODO: This might be able to be a static-sized array */
	struct ncx_char *chars;
};

ncx_shader_t ncx_font_shader_create(const char *vert, const char *frag);
struct ncx_font ncx_font_create(const char *path);
void ncx_font_destroy(struct ncx_font *font);

void ncx_font_draw(struct ncx_font font, const char *string,
		struct ncx_vec2 pos, struct ncx_vec3 color, float scale,
		struct ncx_vec2 window_size, ncx_shader_t shader);

#endif

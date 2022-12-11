#ifndef __NCX_UI_H_
#define __NCX_UI_H_

#include "ncx/tex.h"
#include "ncx/vec2.h"

struct ncx_ui_elem_t {
	struct ncx_vec2 pos;
	struct ncx_vec2 size;
	ncx_tex_t *tex;
	uint8_t tex_count;
};

void ncx_ui_elems_init(const float width, const float height);
void ncx_ui_elems_set_flash(const float flash);

struct ncx_ui_elem_t ncx_ui_elem_create(struct ncx_vec2 pos,
		struct ncx_vec2 size, const ncx_tex_t *texs,
		const uint8_t tex_count);

void ncx_ui_elem_draw(const struct ncx_ui_elem_t ui,
		const uint8_t tex_index);

void ncx_ui_elems_terminate(void);

#endif

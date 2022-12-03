#ifndef __NCX_UI_H_
#define __NCX_UI_H_

#include "narcotix/texture.h"

typedef struct {
	float pos[2];
	float size[2];
	ncx_texture_t *textures;
	uint8_t texture_count;
} ncx_ui_element_t;

void ncx_ui_elements_init(const float width, const float height);
void ncx_ui_elements_set_flash(const float flash);

ncx_ui_element_t ncx_ui_element_create(float *pos, float *size,
		const ncx_texture_t *textures, const uint8_t texture_count);
void ncx_ui_element_draw(const ncx_ui_element_t ui,
		const uint8_t texture_index);

void ncx_ui_elements_terminate(void);

#endif

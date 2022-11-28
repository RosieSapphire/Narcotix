#ifndef __NCX_UI_H_
#define __NCX_UI_H_

#include "narcotix/texture.h"

typedef struct {
	float pos[2];
	float size[2];
	NCXTexture *textures;
	uint8_t texture_count;
} NCXUIElement;

void ncx_ui_elements_init(const float width, const float height);
void ncx_ui_elements_set_flash(const float flash);

NCXUIElement ncx_ui_element_create(float *pos, float *size,
		const NCXTexture *textures, const uint8_t texture_count);
void ncx_ui_element_draw(const NCXUIElement ui, const uint8_t texture_index);
void ncx_ui_element_destroy(NCXUIElement *ui);

void ncx_ui_elements_terminate(void);

#endif

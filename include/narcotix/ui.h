#ifndef NCX_UI_H
#define NCX_UI_H

#include "narcotix/texture.h"

typedef struct {
	float pos[2];
	float size[2];
	NCXTexture *textures;
	uint8_t texture_count;
	uint8_t pad0;
	uint16_t pad1;
	uint32_t pad2;
} NCXUIElement;

void ncx_ui_elements_init(const float width, const float height);
void ncx_ui_elements_set_flash(const float flash);

void ncx_ui_element_create(NCXUIElement *ui, float *pos, float *size, const NCXTexture *textures, const uint8_t texture_count);
void ncx_ui_element_draw(const NCXUIElement ui, const uint8_t texture_index);
void ncx_ui_element_destroy(NCXUIElement *ui);

void ncx_ui_elements_terminate(void);

#endif

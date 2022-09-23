#ifndef NCX_SCREEN_RENDER_BUFFER_H
#define NCX_SCREEN_RENDER_BUFFER_H

#include "narcotix/texture.h"

typedef struct {
	uint32_t fbo;
	uint32_t rbo;
	ncx_texture_t texture;
} ncx_screen_buffer_t;

void ncx_screen_buffer_create_buffers(void);
void ncx_screen_buffer_create_shader(void);
void ncx_screen_buffer_create(ncx_screen_buffer_t *buffer, const int32_t width, const int32_t height, const uint8_t use_alpha);

void ncx_screen_buffer_draw(const ncx_screen_buffer_t buffer, const ncx_texture_t overlay, const float time, const float trip_intensity);

void ncx_screen_buffer_destroy(ncx_screen_buffer_t *buffer);
void ncx_screen_buffer_destroy_shader(void);
void ncx_screen_buffer_destroy_buffers(void);

#endif

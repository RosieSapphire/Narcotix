#ifndef NCX_SCREEN_RENDER_BUFFER_H
#define NCX_SCREEN_RENDER_BUFFER_H

#include "narcotix/texture.h"

typedef struct {
	uint32_t fbo;
	uint32_t rbo;
	NCXTexture texture;
} NCXScreenBuffer;

void ncx_screen_buffer_create_buffers(void);

void ncx_screen_buffer_create_shader_internal(const char *file, const uint32_t line);
#define ncx_screen_buffer_create_shader() ncx_screen_buffer_create_shader_internal(__FILE__, __LINE__)

void ncx_screen_buffer_create(NCXScreenBuffer *buffer, const int32_t width, const int32_t height);

void ncx_screen_buffer_draw(const NCXScreenBuffer buffer, const NCXTexture overlay, const float time, const float trip_intensity);

void ncx_screen_buffer_destroy(NCXScreenBuffer *buffer);
void ncx_screen_buffer_destroy_shader(void);
void ncx_screen_buffer_destroy_buffers(void);

#endif

#ifndef NCX_RENDERER_H
#define NCX_RENDERER_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "cglm/cglm.h"
#include "narcotix/screen_buffer.h"

typedef struct {
	const GLFWvidmode* vidmode;
	GLFWmonitor* monitor;
	GLFWwindow *window;
	vec2 monitor_size;
	vec2 base_size;
	ivec2 window_position;
	uint8_t is_fullscreen;

	uint8_t pad0;
	uint16_t pad1;
	uint32_t pad2;
	ncx_screen_buffer_t *sbos;
	uint32_t sbo_count;
	uint32_t pad3;
} ncx_renderer_t;

void ncx_renderer_create(ncx_renderer_t *ren, const float width, const float height, const uint8_t sbo_count);
void ncx_renderer_center_mouse(ncx_renderer_t *ren);
uint8_t ncx_renderer_key_get_press(const ncx_renderer_t ren, int32_t key);
uint8_t ncx_renderer_mouse_button_get(const ncx_renderer_t ren, int32_t button);
void ncx_renderer_draw(const ncx_renderer_t ren, const ncx_texture_t overlay, const float time, const float trip_intensity);
void ncx_renderer_mouse_pos_get(const ncx_renderer_t ren, vec2 mouse_pos);
void ncx_renderer_mouse_pos_set(ncx_renderer_t *ren, vec2 mouse_pos);
void ncx_renderer_swap_buffers(const ncx_renderer_t ren);
void ncx_renderer_destroy(ncx_renderer_t *ren);

#endif

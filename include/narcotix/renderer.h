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
	NCXScreenBuffer *sbos;
	uint32_t sbo_count;
	uint32_t pad3;
} NCXRenderer;

NCXRenderer ncx_renderer_create(const float width, const float height, const uint8_t sbo_count, const char *window_name, const uint8_t use_blending, const uint8_t use_depth);
void ncx_renderer_center_mouse(NCXRenderer *ren);
uint8_t ncx_renderer_key_get_press(const NCXRenderer ren, int32_t key);
uint8_t ncx_renderer_mouse_button_get(const NCXRenderer ren, int32_t button);
void ncx_renderer_clear_color(const float r, const float g, const float b, const float a);
void ncx_renderer_clear_depth(void);
void ncx_renderer_bind_sbo(const NCXRenderer ren, const uint8_t index);
void ncx_renderer_unbind_sbo(void);
void ncx_renderer_display(const NCXRenderer ren, const NCXTexture overlay, const float time, const float trip_intensity);
void ncx_renderer_mouse_pos_get(const NCXRenderer ren, vec2 mouse_pos);
void ncx_renderer_mouse_pos_set(NCXRenderer *ren, vec2 mouse_pos);
void ncx_renderer_poll(const NCXRenderer ren);
void ncx_renderer_destroy(NCXRenderer *ren);

uint8_t ncx_renderer_running_get(const NCXRenderer ren);
void ncx_renderer_running_set(const NCXRenderer ren, const uint8_t value);

#endif

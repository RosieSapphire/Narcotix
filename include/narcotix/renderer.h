#ifndef NCX_RENDERER_H
#define NCX_RENDERER_H

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "cglm/cglm.h"
#include "narcotix/screen_buffer.h"

typedef struct {
	GLFWwindow *window;
	vec2 window_size;
	ivec2 window_position;

	NCXScreenBuffer *sbos;
	uint32_t sbo_count;
} NCXRenderer;

NCXRenderer ncx_renderer_create_internal(const float width, const float height, const uint8_t sbo_count, const char *window_name, const uint8_t use_blending, const char *file, const uint32_t line);
#define ncx_renderer_create(W, H, SBO_C, WN, UB) ncx_renderer_create_internal(W, H, SBO_C, WN, UB, __FILE__, __LINE__)

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

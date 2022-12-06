#ifndef __NCX_CONTEXT_H_
#define __NCX_CONTEXT_H_

#include "narcotix/shader.h"
#include "narcotix/render_buffer.h"
#include "narcotix/glad/glad.h"
#include "narcotix/vec2.h"
#include <GLFW/glfw3.h>

void ncx_init(const float width, const float height,
		const uint8_t rb_count, const char *window_name,
		const uint8_t use_blending);
void ncx_terminate(void);

float ncx_time(void);
void ncx_time_delta_init(void);
float ncx_time_delta(void);
uint8_t ncx_key_down(const int32_t key);
uint8_t ncx_key_pressed(int32_t key);
uint8_t ncx_key_released(int32_t key);
uint8_t ncx_mouse_button_down(int32_t button);
uint8_t ncx_mouse_button_pressed(int32_t button);
void ncx_mouse_center(void);
ncx_vec2_t ncx_mouse_pos_get(void);
void ncx_mouse_pos_set(ncx_vec2_t new_pos);
void ncx_mouse_input_raw(const uint8_t toggle);
void ncx_clear_color(const float r, const float g,
		const float b, const float a);
void ncx_clear_depth(void);
void ncx_render_buffer_bind(const uint8_t index);
void ncx_render_buffer_unbind(void);
void ncx_buffer_display(const ncx_texture_t overlay, const float time,
		const float trip_intensity);
void ncx_buffer_swap(void);
uint8_t ncx_window_is_running(void);
void ncx_window_close(void);

#endif

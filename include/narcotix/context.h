#ifndef __NCX_CONTEXT_H_
#define __NCX_CONTEXT_H_

#include "narcotix/shader.h"
#include "narcotix/render_buffer.h"
#include "narcotix/glad/glad.h"
#include <GLFW/glfw3.h>

typedef struct {
	GLFWwindow *window;
	vec2 window_size;
	ivec2 window_position;
	
	uint32_t render_quad_vao;
	uint32_t render_quad_vbo;
	ncx_shader_t render_quad_shader;
	
	uint32_t render_buffer_count;
	ncx_render_buffer_t *render_buffers;
} ncx_context_t;

ncx_context_t ncx_context_create(const float width, const float height,
		const uint8_t rb_count, const char *window_name,
		const uint8_t use_blending);
void ncx_context_destroy(ncx_context_t *context);

float ncx_context_time_get(void);
void ncx_context_time_delta_init(void);
float ncx_context_time_delta_get(void);
uint8_t ncx_context_key_get_down(const ncx_context_t context,
		const int32_t key);
uint8_t ncx_context_key_get_pressed(const ncx_context_t context, int32_t key);
uint8_t ncx_context_mouse_button_get_down(const ncx_context_t context,
		int32_t button);
uint8_t ncx_context_mouse_button_get_pressed(const ncx_context_t context,
		int32_t button);
void ncx_context_mouse_center(const ncx_context_t context);
void ncx_context_mouse_pos_get(const ncx_context_t context, vec2 mouse_pos);
void ncx_context_mouse_pos_set(const ncx_context_t context, vec2 mouse_pos);
void ncx_context_mouse_input_raw(const ncx_context_t context,
		const uint8_t toggle);
void ncx_context_clear_color(const float r, const float g,
		const float b, const float a);
void ncx_context_clear_depth(void);
void ncx_context_render_buffer_bind(const ncx_context_t context,
		const uint8_t index);
void ncx_context_render_buffer_unbind(void);
void ncx_context_buffer_display(const ncx_context_t context,
		const ncx_texture_t overlay, const float time,
		const float trip_intensity);
void ncx_context_buffer_swap(const ncx_context_t context);
uint8_t ncx_context_window_is_running(const ncx_context_t context);
void ncx_context_window_close(const ncx_context_t context);

#endif

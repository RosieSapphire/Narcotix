#ifndef __NCX_CONTEXT_H_
#define __NCX_CONTEXT_H_

#include "narcotix/glad/glad.h"
#include "narcotix/shader.h"
#include "narcotix/render_buffer.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

typedef struct {
	GLFWwindow *window;
	vec2 window_size;
	ivec2 window_position;
	
	uint32_t render_quad_vao;
	uint32_t render_quad_vbo;
	NCXShader render_quad_shader;
	
	uint32_t render_buffer_count;
	NCXRenderBuffer *render_buffers;
} NCXContext;

NCXContext ncx_context_create(const float width, const float height,
		const uint8_t rb_count, const char *window_name,
		const uint8_t use_blending);
void ncx_context_destroy(NCXContext *context);

float ncx_context_time_get(void);
void ncx_context_time_delta_init(void);
float ncx_context_time_delta_get(void);
uint8_t ncx_context_key_get_down(const NCXContext context, const int32_t key);
uint8_t ncx_context_key_get_pressed(const NCXContext context, int32_t key);
uint8_t ncx_context_mouse_button_get_down(const NCXContext context,
		int32_t button);
uint8_t ncx_context_mouse_button_get_pressed(const NCXContext context,
		int32_t button);
void ncx_context_mouse_center(const NCXContext context);
void ncx_context_mouse_pos_get(const NCXContext context, vec2 mouse_pos);
void ncx_context_mouse_pos_set(const NCXContext context, vec2 mouse_pos);
void ncx_context_mouse_input_raw(const NCXContext context,
		const uint8_t toggle);
void ncx_context_clear_color(const float r, const float g,
		const float b, const float a);
void ncx_context_clear_depth(void);
void ncx_context_render_buffer_bind(const NCXContext context,
		const uint8_t index);
void ncx_context_render_buffer_unbind(void);
void ncx_context_buffer_display(const NCXContext context,
		const NCXTexture overlay, const float time,
		const float trip_intensity);
void ncx_context_buffer_swap(const NCXContext context);
uint8_t ncx_context_window_is_running(const NCXContext context);
void ncx_context_window_close(const NCXContext context);

#endif

#include "narcotix/context.h"
#include <assert.h>

#ifdef DEBUG
	#include "narcotix/debug.h"
#endif

static float time_last = 0.0f;

float ncx_context_time_get(void) {
	return (float)glfwGetTime();
}

void ncx_context_time_delta_init(void) {
	time_last = ncx_context_time_get();
}

float ncx_context_time_delta_get(void) {
	float time_now = ncx_context_time_get();
	float time_delta = time_now - time_last;
	time_last = time_now;
	return time_delta;
}

void ncx_context_mouse_center(const NCXContext context) {
	glfwSetInputMode(context.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(context.window,
			(double)context.window_size[0] / 2,
			(double)context.window_size[1] / 2);
}

uint8_t ncx_context_key_get(const NCXContext context, const int32_t key) {
	return (uint8_t)glfwGetKey(context.window, key);
}

uint8_t ncx_context_key_get_press(const NCXContext context, int32_t key) {
	return ncx_context_key_get(context, key) == GLFW_PRESS;
}

uint8_t ncx_context_key_get_release(const NCXContext context, int32_t key) {
	return ncx_context_key_get(context, key) == GLFW_RELEASE;
}

uint8_t ncx_context_mouse_button_get(const NCXContext context, int32_t button) {
	return (uint8_t)glfwGetMouseButton(context.window, button);
}

void ncx_context_mouse_pos_get(const NCXContext context, vec2 mouse_pos) {
	double mouse_x, mouse_y;
	glfwGetCursorPos(context.window, &mouse_x, &mouse_y);
	mouse_pos[0] = (float)mouse_x;
	mouse_pos[1] = (float)mouse_y;
}

void ncx_context_mouse_pos_set(const NCXContext context, vec2 mouse_pos) {
	glfwSetCursorPos(context.window,
			(double)mouse_pos[0], (double)mouse_pos[1]);
}

void ncx_context_mouse_input_raw(const NCXContext context,
		const uint8_t toggle) {
	assert(glfwRawMouseMotionSupported());
	glfwSetInputMode(context.window, GLFW_RAW_MOUSE_MOTION, toggle);
}

void ncx_context_clear_color(const float r, const float g,
		const float b, const float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void ncx_context_clear_depth(void) {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ncx_context_render_buffer_bind(const NCXContext context,
		const uint8_t index) {
	assert(index < context.render_buffer_count);
	glBindFramebuffer(GL_FRAMEBUFFER, context.render_buffers[index].fbo);
}

void ncx_context_render_buffer_unbind(void) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ncx_context_buffer_display(const NCXContext context,
		const NCXTexture overlay, const float time,
		const float trip_intensity) {
	for(uint8_t i = 0; i < context.render_buffer_count; i++) {
		glDisable(GL_DEPTH_TEST);
		ncx_shader_use(context.render_quad_shader);
		ncx_shader_uniform_int(context.render_quad_shader, "screen_texture", 0);
		ncx_shader_uniform_int(context.render_quad_shader, "trippy_texture", 1);
		ncx_shader_uniform_int(context.render_quad_shader,
				"use_trippy_effect", overlay);
		ncx_shader_uniform_float(context.render_quad_shader, "time", time);
		ncx_shader_uniform_float(context.render_quad_shader,
				"trip_intensity", trip_intensity);

		glBindVertexArray(context.render_quad_vao);
		ncx_texture_use(context.render_buffers[i].texture, 0);
		ncx_texture_use(overlay, 1);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		ncx_texture_use(0, 0);
		ncx_texture_use(0, 1);
		glBindVertexArray(0);
		glEnable(GL_DEPTH_TEST);
	}
}

void ncx_context_buffer_swap(const NCXContext context) {
	glfwSwapBuffers(context.window);
	glfwPollEvents();
}

uint8_t ncx_context_window_is_running(const NCXContext context) {
	return !glfwWindowShouldClose(context.window);
}

void ncx_context_window_close(const NCXContext context) {
	glfwSetWindowShouldClose(context.window, 1);
}

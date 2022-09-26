#include "narcotix/renderer.h"
#include "narcotix/helpers.h"
#include <GLFW/glfw3.h>

#include <assert.h>

#include "narcotix/debug.h"

NCXRenderer ncx_renderer_create_internal(const float width, const float height, const uint8_t sbo_count, const char *window_name, const uint8_t use_blending, const char *file, const uint32_t line) {
	NCXRenderer ren;
	#ifdef DEBUG
		if(!glfwInit()) {
			printf("%sNARCOTIX::GLFW::ERROR: %sGLFW initialization fucked up. %s(Caused at '%s' line %i)\n", D_COLOR_RED, D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
			ren.monitor = NULL;
			return ren;
		}
	#else
		glfwInit();
	#endif

	glfwSetErrorCallback((void *)&glfw_error_callback);
	ren.monitor = glfwGetPrimaryMonitor();
	ren.vidmode = glfwGetVideoMode(ren.monitor);
	ren.sbo_count = sbo_count;
	glm_vec2_copy((vec2){(float)ren.vidmode->width, (float)ren.vidmode->height}, ren.monitor_size);
	glm_vec2_copy((vec2){width, height}, ren.base_size);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	ren.window = glfwCreateWindow((int32_t)width, (int32_t)height, window_name, NULL, NULL);
	#ifdef DEBUG
		if(!ren.window) {
			fprintf(stderr, "%sNARCOTIX::WINDOW::ERROR: %sWindow creation fucked up. %s(Caused at '%s' line %i)\n", D_COLOR_RED, D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
			glfwTerminate();
			return ren;
		}
	#endif
	glfwMakeContextCurrent(ren.window);

	ren.window_position[0] = (int32_t)((ren.monitor_size[0] / 2) - (width / 2));
	ren.window_position[1] = (int32_t)((ren.monitor_size[1] / 2) - (height / 2));
	glfwSetWindowPos(ren.window, ren.window_position[0], ren.window_position[1]);

	#ifdef DEBUG
		if(!gladLoadGL()) {
			fprintf(stderr, "%sNARCOTIX::GLAD::ERROR: %sGLAD failed to load OpenGL functions. %s(Caused at '%s' line %i)\n", D_COLOR_RED, D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
			glfwTerminate();
			return ren;
		}
	#else 
		gladLoadGL();
	#endif

	ncx_screen_buffer_create_buffers();
	ncx_screen_buffer_create_shader();
	ren.sbos = calloc(sbo_count, sizeof(NCXScreenBuffer));
	for(uint8_t i = 0; i < ren.sbo_count; i++) {
		ncx_screen_buffer_create(&ren.sbos[i], (int32_t)ren.base_size[0], (int32_t)ren.base_size[1]);
	}
	glViewport(0, 0, (int32_t)width, (int32_t)height);

	if(use_blending) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	return ren;
}

void ncx_renderer_center_mouse(NCXRenderer *ren) {
	glfwSetInputMode(ren->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(ren->window, (double)ren->base_size[0] / 2, (double)ren->base_size[1] / 2);
}

uint8_t ncx_renderer_key_get_press(const NCXRenderer ren, int32_t key) {
	return glfwGetKey(ren.window, key) == GLFW_PRESS;
}

uint8_t ncx_renderer_mouse_button_get(const NCXRenderer ren, int32_t button) {
	return (uint8_t)glfwGetMouseButton(ren.window, button);
}


void ncx_renderer_mouse_pos_get(const NCXRenderer ren, vec2 mouse_pos) {
	double mouse_x, mouse_y;
	glfwGetCursorPos(ren.window, &mouse_x, &mouse_y);
	mouse_pos[0] = (float)mouse_x;
	mouse_pos[1] = (float)mouse_y;
}

void ncx_renderer_mouse_pos_set(NCXRenderer *ren, vec2 mouse_pos) {
	glfwSetCursorPos(ren->window, (double)mouse_pos[0], (double)mouse_pos[1]);
}

void ncx_renderer_clear_color(const float r, const float g, const float b, const float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void ncx_renderer_clear_depth(void) {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ncx_renderer_bind_sbo(const NCXRenderer ren, const uint8_t index) {
	assert(index < ren.sbo_count);
	glBindFramebuffer(GL_FRAMEBUFFER, ren.sbos[index].fbo);
}

void ncx_renderer_unbind_sbo() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ncx_renderer_display(const NCXRenderer ren, const NCXTexture overlay, const float time, const float trip_intensity) {
	for(uint8_t i = 0; i < ren.sbo_count; i++) {
		ncx_screen_buffer_draw(ren.sbos[i], overlay, time, trip_intensity);
	}
}

void ncx_renderer_poll(const NCXRenderer ren) {
	glfwSwapBuffers(ren.window);
	glfwPollEvents();
}

void ncx_renderer_destroy(NCXRenderer *ren) {
	for(uint8_t i = 0; i < ren->sbo_count; i++) {
		ncx_screen_buffer_destroy(&ren->sbos[i]);
	}
	ncx_screen_buffer_destroy_shader();
	ncx_screen_buffer_destroy_buffers();
	free(ren->sbos);
	glfwDestroyWindow(ren->window);
	glfwTerminate();
}

uint8_t ncx_renderer_running_get(const NCXRenderer ren) {
	return !glfwWindowShouldClose(ren.window);
}

void ncx_renderer_running_set(const NCXRenderer ren, const uint8_t value) {
	glfwSetWindowShouldClose(ren.window, value);
}

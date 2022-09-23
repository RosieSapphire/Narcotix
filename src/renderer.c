#include "narcotix/renderer.h"

void ncx_renderer_create(ncx_renderer_t *ren, const float width, const float height, const uint8_t sbo_count) {
	#ifdef DEBUG
		if(!glfwInit()) {
			printf("ERROR: GLFW fucked up.\n");
			return;
		}
	#else
		glfwInit();
	#endif

	ren->monitor = glfwGetPrimaryMonitor();
	ren->vidmode = glfwGetVideoMode(ren->monitor);
	ren->sbo_count = sbo_count;
	glm_vec2_copy((vec2){(float)ren->vidmode->width, (float)ren->vidmode->height}, ren->monitor_size);
	glm_vec2_copy((vec2){width, height}, ren->base_size);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_RED_BITS, ren->vidmode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, ren->vidmode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, ren->vidmode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, ren->vidmode->refreshRate);
	ren->window = glfwCreateWindow((int32_t)width, (int32_t)height, "Hungover", NULL, NULL);
	#ifdef DEBUG
		if(!ren->window) {
			fprintf(stderr, "ERROR: Window fucked up.\n");
			glfwTerminate();
			return;
		}
	#endif
	glfwMakeContextCurrent(ren->window);

	ren->window_position[0] = (int32_t)((ren->monitor_size[0] / 2) - (width / 2));
	ren->window_position[1] = (int32_t)((ren->monitor_size[1] / 2) - (height / 2));
	glfwSetWindowPos(ren->window, ren->window_position[0], ren->window_position[1]);

	#ifdef DEBUG
		if(!gladLoadGL()) {
			printf("ERROR: GLAD fucked up.\n");
			glfwTerminate();
			return;
		}
	#else 
		gladLoadGL();
	#endif

	ncx_screen_buffer_create_buffers();
	ncx_screen_buffer_create_shader();
	ren->sbos = calloc(sbo_count, sizeof(ncx_screen_buffer_t));
	for(uint8_t i = 0; i < ren->sbo_count; i++) {
		ncx_screen_buffer_create(&ren->sbos[i], (int32_t)ren->base_size[0], (int32_t)ren->base_size[1], i % 2);
	}
	glViewport(0, 0, (int32_t)width, (int32_t)height);
}

void ncx_renderer_center_mouse(ncx_renderer_t *ren) {
	glfwSetInputMode(ren->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(ren->window, (double)ren->base_size[0] / 2, (double)ren->base_size[1] / 2);
}

uint8_t ncx_renderer_key_get_press(const ncx_renderer_t ren, int32_t key) {
	return glfwGetKey(ren.window, key) == GLFW_PRESS;
}

uint8_t ncx_renderer_mouse_button_get(const ncx_renderer_t ren, int32_t button) {
	return (uint8_t)glfwGetMouseButton(ren.window, button);
}


void ncx_renderer_mouse_pos_get(const ncx_renderer_t ren, vec2 mouse_pos) {
	double mouse_x, mouse_y;
	glfwGetCursorPos(ren.window, &mouse_x, &mouse_y);
	mouse_pos[0] = (float)mouse_x;
	mouse_pos[1] = (float)mouse_y;
}

void ncx_renderer_mouse_pos_set(ncx_renderer_t *ren, vec2 mouse_pos) {
	glfwSetCursorPos(ren->window, (double)mouse_pos[0], (double)mouse_pos[1]);
}

void ncx_renderer_draw(const ncx_renderer_t ren, const ncx_texture_t overlay, const float time, const float trip_intensity) {
	for(uint8_t i = 0; i < ren.sbo_count; i++) {
		ncx_screen_buffer_draw(ren.sbos[i], overlay, time, trip_intensity);
	}
}

void ncx_renderer_swap_buffers(const ncx_renderer_t ren) {
	glfwSwapBuffers(ren.window);
	glfwPollEvents();
}

void ncx_renderer_destroy(ncx_renderer_t *ren) {
	for(uint8_t i = 0; i < ren->sbo_count; i++) {
		ncx_screen_buffer_destroy(&ren->sbos[i]);
	}
	ncx_screen_buffer_destroy_shader();
	ncx_screen_buffer_destroy_buffers();
	free(ren->sbos);
	glfwDestroyWindow(ren->window);
	glfwTerminate();
}

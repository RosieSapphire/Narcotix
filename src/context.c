#include "narcotix/context.h"
#include "narcotix/helpers.h"
#include <assert.h>

static float time_last = 0.0f;
static uint8_t key_states[512];
static uint8_t mouse_states[2];

NCXContext ncx_context_create(const float width, const float height,
		const uint8_t rb_count, const char *window_name,
		const uint8_t use_blending) {

	uint32_t glfw_status = glfwInit();
	assert(glfw_status);

	glfwSetErrorCallback((void *)&glfw_error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	NCXContext context;
	context.window = glfwCreateWindow((int32_t)width, (int32_t)height,
			window_name, NULL, NULL);
	assert(context.window);
	glfwMakeContextCurrent(context.window);

	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);
	context.window_position[0] =
		(int32_t)((vidmode->width / 2) - (width / 2));
	context.window_position[1] =
		(int32_t)((vidmode->height / 2) - (height / 2));
	glfwSetWindowPos(context.window, context.window_position[0],
			context.window_position[1]);

	context.window_size[0] = width;
	context.window_size[1] = height;

	uint32_t glad_status = gladLoadGL();
	assert(glad_status);

	/* set up the render buffer vertex data */
	glGenVertexArrays(1, &context.render_quad_vao);
	glBindVertexArray(context.render_quad_vao);

	glGenBuffers(1, &context.render_quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, context.render_quad_vbo);

	const float render_quad_vertices[] = {
		 1.0f, -1.0f,	1.0f, 0.0f,
		-1.0f,  1.0f,	0.0f, 1.0f,
		-1.0f, -1.0f,	0.0f, 0.0f,
	
		 1.0f, -1.0f,	1.0f, 0.0f,
		 1.0f,  1.0f,	1.0f, 1.0f,
		-1.0f,  1.0f,	0.0f, 1.0f,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(render_quad_vertices),
			render_quad_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/* load in the render buffer's shader */
	context.render_quad_shader = ncx_shader_create(
			"res/shaders/builtin/screen_vert.glsl", NULL,
			"res/shaders/builtin/screen_frag.glsl");

	context.render_buffer_count = rb_count;
	context.render_buffers =
		malloc(context.render_buffer_count * sizeof(NCXRenderBuffer));

	for(uint8_t i = 0; i < context.render_buffer_count; i++) {
		NCXRenderBuffer *rb_cur = context.render_buffers + i;
		glGenFramebuffers(1, &rb_cur->fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, rb_cur->fbo);

		glGenTextures(1, &rb_cur->texture);
		glBindTexture(GL_TEXTURE_2D, rb_cur->texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
				0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, rb_cur->texture, 0);

		glGenRenderbuffers(1, &rb_cur->rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rb_cur->rbo);
		glRenderbufferStorage(GL_RENDERBUFFER,
				GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,
				GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rb_cur->rbo);

		#ifdef DEBUG
			if(glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
					GL_FRAMEBUFFER_COMPLETE) {
				printf("ERROR: Framebuffer fucked up\n");
				assert(0);
			}
		#endif

		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	glViewport(0, 0, (int32_t)width, (int32_t)height);

	if(use_blending) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	return context;
}

void ncx_context_destroy(NCXContext *context) {
	NCXRenderBuffer *rb_end = context->render_buffers +
		context->render_buffer_count;
	for(NCXRenderBuffer *rb_cur = context->render_buffers;
			rb_cur < rb_end; rb_cur++) {
		glDeleteFramebuffers(1, &rb_cur->fbo);
		glDeleteRenderbuffers(1, &rb_cur->rbo);
		glDeleteTextures(1, &rb_cur->texture);
	}
	free(context->render_buffers);

	glfwDestroyWindow(context->window);
	glfwTerminate();
}

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

uint8_t ncx_context_key_get_down(const NCXContext context, const int32_t key) {
	key_states[key] = glfwGetKey(context.window, key);
	return key_states[key];
}

uint8_t ncx_context_key_get_pressed(const NCXContext context, int32_t key) {
	uint8_t key_state_new = glfwGetKey(context.window, key);
	uint8_t key_pressed_now = key_state_new && !key_states[key];
	key_states[key] = key_state_new;
	return key_pressed_now;
}

uint8_t ncx_context_mouse_button_get_down(const NCXContext context,
		int32_t button) {

	mouse_states[button] = glfwGetMouseButton(context.window, button);
	return mouse_states[button];
}

uint8_t ncx_context_mouse_button_get_pressed(const NCXContext context,
		int32_t button) {
	uint8_t mouse_state_new = glfwGetMouseButton(context.window, button);
	uint8_t mouse_pressed_now = mouse_state_new && !mouse_states[button];
	mouse_states[button] = mouse_state_new;
	return mouse_pressed_now;
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
	if(ncx_context_key_get_pressed(context, GLFW_KEY_ESCAPE)) {
		ncx_context_window_close(context);
		return 0;
	}

	return !glfwWindowShouldClose(context.window);
}

void ncx_context_window_close(const NCXContext context) {
	glfwSetWindowShouldClose(context.window, 1);
}

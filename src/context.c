#include "narcotix/context.h"
#include "narcotix/helpers.h"
#include "narcotix/ivec2.h"

#include <assert.h>
#include <stdio.h>
#include <malloc.h>

static GLFWwindow *window;
static struct ncx_vec2 window_size;
static struct ncx_ivec2 window_position;

static uint32_t render_quad_vao;
static uint32_t render_quad_vbo;
static ncx_shader_t render_quad_shader;

static uint32_t render_buffer_count;
static ncx_render_buffer_t *render_buffers;

static float time_last = 0.0f;
static uint8_t key_states[512];
static uint8_t mouse_states[2];

void ncx_init(const float width, const float height,
		const uint8_t rb_count, const char *window_name,
		const uint8_t use_blending)
{

	if(!glfwInit()) {
		fprintf(stderr, "GLFW ERROR: GLFW failed to initialize.\n");
		assert(0);
	}

	glfwSetErrorCallback((void *)&glfw_error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow((int32_t)width, (int32_t)height,
			window_name, NULL, NULL);
	if(!window) {
		fprintf(stderr, "GLFW ERROR: Failed to create window '%s' "
				"(%d x %d)\n", window_name, (int)width,
				(int)height);
		assert(0);
	}

	glfwMakeContextCurrent(window);

	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);
	window_position.x =
		(int32_t)((vidmode->width / 2) - (width / 2));
	window_position.y =
		(int32_t)((vidmode->height / 2) - (height / 2));
	glfwSetWindowPos(window, window_position.x,
			window_position.y);

	window_size.x = width;
	window_size.y = height;

	if(!gladLoadGL()) {
		fprintf(stderr, "OPENGL ERROR: Glad failed"
				" to load OpenGL Functions.\n");
		assert(0);
	}

	/* set up the render buffer vertex data */
	glGenVertexArrays(1, &render_quad_vao);
	glBindVertexArray(render_quad_vao);

	glGenBuffers(1, &render_quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, render_quad_vbo);

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

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
			4 * sizeof(float), NULL);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/* load in the render buffer's shader */
	render_quad_shader = ncx_shader_create(
			"res/shdr/internal/screen_vert.glsl", NULL,
			"res/shdr/internal/screen_frag.glsl");

	render_buffer_count = rb_count;
	render_buffers =
		malloc(render_buffer_count * sizeof(ncx_render_buffer_t));

	for(uint8_t i = 0; i < render_buffer_count; i++) {
		ncx_render_buffer_t *rb_cur = render_buffers + i;
		glGenFramebuffers(1, &rb_cur->fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, rb_cur->fbo);

		glGenTextures(1, &rb_cur->tex);
		glBindTexture(GL_TEXTURE_2D, rb_cur->tex);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
				0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
				GL_MIRRORED_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
				GL_MIRRORED_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D, rb_cur->tex, 0);

		glGenRenderbuffers(1, &rb_cur->rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rb_cur->rbo);

		glRenderbufferStorage(GL_RENDERBUFFER,
				GL_DEPTH24_STENCIL8, width, height);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER,
				GL_DEPTH_STENCIL_ATTACHMENT,
				GL_RENDERBUFFER, rb_cur->rbo);

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
}

void ncx_terminate()
{
	ncx_render_buffer_t *rb_end = render_buffers +
		render_buffer_count;
	for(ncx_render_buffer_t *rb_cur = render_buffers;
			rb_cur < rb_end; rb_cur++) {
		glDeleteFramebuffers(1, &rb_cur->fbo);
		glDeleteRenderbuffers(1, &rb_cur->rbo);
		glDeleteTextures(1, &rb_cur->tex);
	}
	free(render_buffers);

	glfwDestroyWindow(window);
	glfwTerminate();
}

float ncx_time(void)
{
	return (float)glfwGetTime();
}

void ncx_time_delta_init(void)
{
	time_last = ncx_time();
}

float ncx_time_delta(void)
{
	float time_now = ncx_time();
	float time_delta = time_now - time_last;
	time_last = time_now;
	return time_delta;
}

void ncx_mouse_center()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window,
			(double)window_size.x / 2,
			(double)window_size.y / 2);
}

uint8_t ncx_key_down(const int32_t key)
{
	key_states[key] = glfwGetKey(window, key);
	return key_states[key];
}

uint8_t ncx_key_pressed(int32_t key)
{
	uint8_t key_state_new = glfwGetKey(window, key);
	uint8_t key_pressed_now = key_state_new && !key_states[key];
	key_states[key] = key_state_new;
	return key_pressed_now;
}

uint8_t ncx_key_released(int32_t key)
{
	uint8_t key_state_new = glfwGetKey(window, key);
	uint8_t key_released_now = !key_state_new && key_states[key];
	key_states[key] = key_state_new;
	return key_released_now;
}

uint8_t ncx_mouse_button_down(int32_t button)
{
	mouse_states[button] = glfwGetMouseButton(window, button);
	return mouse_states[button];
}

uint8_t ncx_mouse_button_pressed(int32_t button)
{
	uint8_t mouse_state_new = glfwGetMouseButton(window, button);
	uint8_t mouse_pressed_now = mouse_state_new && !mouse_states[button];
	mouse_states[button] = mouse_state_new;
	return mouse_pressed_now;
}

struct ncx_vec2 ncx_mouse_pos_get(void)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	return (struct ncx_vec2){x, y};
}

void ncx_mouse_pos_set(struct ncx_vec2 new_pos)
{
	glfwSetCursorPos(window, new_pos.x, new_pos.y);
}

void ncx_mouse_input_raw(const uint8_t toggle)
{
	assert(glfwRawMouseMotionSupported());
	glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, toggle);
}

void ncx_clear_color(const float r, const float g,
		const float b, const float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void ncx_clear_depth(void)
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ncx_render_buffer_bind(const uint8_t index)
{
	assert(index < render_buffer_count);
	glBindFramebuffer(GL_FRAMEBUFFER, render_buffers[index].fbo);
}

void ncx_render_buffer_unbind(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ncx_buffer_display(const ncx_tex_t overlay, const float time,
		const float trip_intensity)
{
	for(uint8_t i = 0; i < render_buffer_count; i++) {
		glDisable(GL_DEPTH_TEST);
		ncx_shader_use(render_quad_shader);
		ncx_shader_uniform_int(render_quad_shader, "screen_tex", 0);
		ncx_shader_uniform_int(render_quad_shader, "trippy_tex", 1);
		ncx_shader_uniform_int(render_quad_shader,
				"use_trippy_effect", overlay);
		ncx_shader_uniform_float(render_quad_shader, "time", time);
		ncx_shader_uniform_float(render_quad_shader,
				"trip_intensity", trip_intensity);

		glBindVertexArray(render_quad_vao);
		ncx_tex_use(render_buffers[i].tex, 0);
		ncx_tex_use(overlay, 1);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		ncx_tex_use(0, 0);
		ncx_tex_use(0, 1);
		glBindVertexArray(0);
		glEnable(GL_DEPTH_TEST);
	}
}

void ncx_buffer_swap()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

uint8_t ncx_window_is_running()
{
	if(ncx_key_pressed(GLFW_KEY_ESCAPE)) {
		ncx_window_close();
		return 0;
	}

	return !glfwWindowShouldClose(window);
}

void ncx_window_close()
{
	glfwSetWindowShouldClose(window, 1);
}

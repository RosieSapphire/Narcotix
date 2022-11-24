#include "narcotix/glad/glad.h"
#include "narcotix/shader.h"
#include "narcotix/render_buffer.h"
#include "narcotix/helpers.h"

#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#ifdef DEBUG
	#include "narcotix/debug.h"
#endif

static GLFWwindow *window;
static vec2 window_size;
static ivec2 window_position;

static uint32_t render_quad_vao;
static uint32_t render_quad_vbo;
static NCXShader render_quad_shader;

static uint32_t render_buffer_count;
static NCXRenderBuffer *render_buffers;

void ncx_init_internal(const float width, const float height,
		const uint8_t rb_count, const char *window_name,
		const uint8_t use_blending, const char *file, const uint32_t line) {
	/* load up GLFW and GLAD, then set up a window */
	#ifdef DEBUG
		if(!glfwInit()) {
			printf("%sNARCOTIX::GLFW::ERROR: %sGLFW initialization fucked up."
					" %s(Caused at '%s' line %u)\n", D_COLOR_RED,
					D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
			assert(0);
		}
	#else
		glfwInit();
	#endif

	GLFWmonitor *monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);

	glfwSetErrorCallback((void *)&glfw_error_callback);

	render_buffer_count = rb_count;
	window_size[0] = width;
	window_size[1] = height;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow((int32_t)width, (int32_t)height,
			window_name, NULL, NULL);

	#ifdef DEBUG
		if(!window) {
			fprintf(stderr, "%sNARCOTIX::WINDOW::ERROR:"
					" %sWindow creation fucked up. %s"
					"(Caused at '%s' line %u)\n", D_COLOR_RED, D_COLOR_YELLOW,
					D_COLOR_DEFAULT, file, line);
			glfwTerminate();
			assert(0);
		}
	#endif

	glfwMakeContextCurrent(window);

	window_position[0] = (int32_t)((vidmode->width / 2) - (width / 2));
	window_position[1] = (int32_t)((vidmode->height / 2) - (height / 2));
	glfwSetWindowPos(window, window_position[0],
			window_position[1]);

	#ifdef DEBUG
		if(!gladLoadGL()) {
			fprintf(stderr, "%sNARCOTIX::GLAD::ERROR: %sGLAD failed to load"
					" OpenGL functions. %s(Caused at '%s' line %u)\n",
					D_COLOR_RED, D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
			glfwTerminate();
			assert(0);
		}
	#else 
		gladLoadGL();
	#endif

	/* set up the render buffer vertex data */
	const float render_quad_vertices[] = {
		 1.0f, -1.0f,	1.0f, 0.0f,
		-1.0f,  1.0f,	0.0f, 1.0f,
		-1.0f, -1.0f,	0.0f, 0.0f,
	
		 1.0f, -1.0f,	1.0f, 0.0f,
		 1.0f,  1.0f,	1.0f, 1.0f,
		-1.0f,  1.0f,	0.0f, 1.0f,
	};

	glGenVertexArrays(1, &render_quad_vao);
	glBindVertexArray(render_quad_vao);

	glGenBuffers(1, &render_quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, render_quad_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(render_quad_vertices),
			render_quad_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/* load in the render buffer's shader */
	render_quad_shader = ncx_shader_create_internal(
			"res/shaders/screen_vert.glsl",
			"res/shaders/screen_frag.glsl", NULL, file, line);
	render_buffers = malloc(render_buffer_count * sizeof(NCXRenderBuffer));
	for(uint8_t i = 0; i < render_buffer_count; i++) {
		NCXRenderBuffer *rb_cur = render_buffers + i;
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

	// glEnable(GL_MULTISAMPLE);

	#ifdef DEBUG
		printf("%sNARCOTIX::RENDERER::CREATE: %sRenderer has successfully been"
				" created, as well as loading GLFW and GLAD. %s"
				"(Caused at %s line %u)\n", D_COLOR_GREEN, D_COLOR_YELLOW,
				D_COLOR_DEFAULT, file, line);
	#endif
}

float ncx_time_get(void) {
	return (float)glfwGetTime();
}

void ncx_mouse_center(void) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(window,
			(double)window_size[0] / 2, (double)window_size[1] / 2);
}

uint8_t ncx_key_get(int32_t key) {
	return (uint8_t)glfwGetKey(window, key);
}

uint8_t ncx_mouse_button_get(int32_t button) {
	return (uint8_t)glfwGetMouseButton(window, button);
}

void ncx_mouse_pos_get(vec2 mouse_pos) {
	double mouse_x, mouse_y;
	glfwGetCursorPos(window, &mouse_x, &mouse_y);
	mouse_pos[0] = (float)mouse_x;
	mouse_pos[1] = (float)mouse_y;
}

void ncx_mouse_pos_set(vec2 mouse_pos) {
	glfwSetCursorPos(window, (double)mouse_pos[0], (double)mouse_pos[1]);
}

void ncx_clear_color(const float r, const float g,
		const float b, const float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void ncx_clear_depth(void) {
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ncx_render_buffer_bind(const uint8_t index) {
	assert(index < render_buffer_count);
	glBindFramebuffer(GL_FRAMEBUFFER, render_buffers[index].fbo);
}

void ncx_render_buffer_unbind(void) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ncx_buffer_display(const NCXTexture overlay, const float time,
		const float trip_intensity) {
	for(uint8_t i = 0; i < render_buffer_count; i++) {
		glDisable(GL_DEPTH_TEST);
		glUseProgram(render_quad_shader);
		glUniform1i(
				glGetUniformLocation(render_quad_shader, "screen_texture"), 0);
		glUniform1i(
				glGetUniformLocation(render_quad_shader, "trippy_texture"), 1);
		glUniform1i(
				glGetUniformLocation(render_quad_shader, "use_trippy_effect"),
				(overlay > 0));
		glUniform1f(glGetUniformLocation(render_quad_shader, "time"), time);
		glUniform1f(
				glGetUniformLocation(render_quad_shader, "trip_intensity"),
				trip_intensity);

		glBindVertexArray(render_quad_vao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, render_buffers[i].texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, overlay);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);
		glEnable(GL_DEPTH_TEST);
	}
}

void ncx_buffer_swap(void) {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void ncx_terminate(void) {
	NCXRenderBuffer *rb_end = render_buffers + render_buffer_count;
	for(NCXRenderBuffer *rb_cur = render_buffers; rb_cur < rb_end; rb_cur++) {
		glDeleteFramebuffers(1, &rb_cur->fbo);
		glDeleteRenderbuffers(1, &rb_cur->rbo);
		glDeleteTextures(1, &rb_cur->texture);
	}
	free(render_buffers);

	glfwDestroyWindow(window);
	glfwTerminate();
}

uint8_t ncx_window_is_running(void) {
	return !glfwWindowShouldClose(window);
}

void ncx_window_close(void) {
	glfwSetWindowShouldClose(window, 0);
}

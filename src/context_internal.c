#include "narcotix/context_internal.h"
#include "narcotix/helpers.h"

#ifdef DEBUG
#include "narcotix/debug.h"
#endif

NCXContext ncx_context_create_internal(const float width, const float height,
		const uint8_t rb_count, const char *window_name,
		const uint8_t use_blending, const char *file, const uint32_t line) {

	#ifdef DEBUG
		if(!glfwInit()) {
			printf("%sNARCOTIX::GLFW::ERROR: %sGLFW createialization fucked up."
					" %s(Caused at '%s' line %u)\n", D_COLOR_RED,
					D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
			assert(0);
		}
	#else
		glfwInit();
	#endif

	glfwSetErrorCallback((void *)&glfw_error_callback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	NCXContext context;
	context.window = glfwCreateWindow((int32_t)width, (int32_t)height,
			window_name, NULL, NULL);

	#ifdef DEBUG
		if(!context.window) {
			fprintf(stderr, "%sNARCOTIX::WINDOW::ERROR:"
					" %sWindow creation fucked up. %s"
					"(Caused at '%s' line %u)\n", D_COLOR_RED, D_COLOR_YELLOW,
					D_COLOR_DEFAULT, file, line);
			glfwTerminate();
			assert(0);
		}
	#endif

	glfwMakeContextCurrent(context.window);

	{
		GLFWmonitor *monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode *vidmode = glfwGetVideoMode(monitor);
		context.window_position[0] =
			(int32_t)((vidmode->width / 2) - (width / 2));
		context.window_position[1] =
			(int32_t)((vidmode->height / 2) - (height / 2));
		glfwSetWindowPos(context.window, context.window_position[0],
				context.window_position[1]);
	}

	context.window_size[0] = width;
	context.window_size[1] = height;

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
	glGenVertexArrays(1, &context.render_quad_vao);
	glBindVertexArray(context.render_quad_vao);

	glGenBuffers(1, &context.render_quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, context.render_quad_vbo);

	{
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
	}

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/* load in the render buffer's shader */
	context.render_quad_shader = ncx_shader_create_internal(
			"res/shaders/builtin/screen_vert.glsl", NULL,
			"res/shaders/builtin/screen_frag.glsl", file, line);

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

	#ifdef DEBUG
		printf("%sNARCOTIX::RENDERER::CREATE: %sRenderer has successfully been"
				" created, as well as loading GLFW and GLAD. %s"
				"(Caused at %s line %u)\n", D_COLOR_GREEN, D_COLOR_YELLOW,
				D_COLOR_DEFAULT, file, line);
	#endif

	return context;
}

void ncx_context_destroy_internal(NCXContext *context, char *file,
		uint32_t line) {
	NCXRenderBuffer *rb_end = context->render_buffers +
		context->render_buffer_count;
	for(NCXRenderBuffer *rb_cur = context->render_buffers;
			rb_cur < rb_end; rb_cur++) {
		glDeleteFramebuffers(1, &rb_cur->fbo);
		glDeleteRenderbuffers(1, &rb_cur->rbo);
		glDeleteTextures(1, &rb_cur->texture);
	}
	free(context->render_buffers);

	#ifdef DEBUG
	printf("%sNARCOTIX::CONTEXT::TERMINATE: %sSuccessfully destroyed Narcotix "
			"Context. %s(Caused at '%s' line %d)\n", D_COLOR_GREEN,
			D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
	#endif

	glfwDestroyWindow(context->window);
	glfwTerminate();
}

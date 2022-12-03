#ifndef __NCX_CONTEXT_INTERNAL_H_
#define __NCX_CONTEXT_INTERNAL_H_

#include "narcotix/glad/glad.h"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "narcotix/shader.h"
#include "narcotix/render_buffer.h"

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

NCXContext ncx_context_create_internal(const float width, const float height,
		const uint8_t rb_count, const char *window_name,
		const uint8_t use_blending, const char *file, const uint32_t line);

void ncx_context_destroy_internal(NCXContext *context, char *file,
		uint32_t line);

#endif

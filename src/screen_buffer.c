#include "narcotix/screen_buffer.h"

#include <stdio.h>
#include "narcotix/glad/glad.h"
#include "narcotix/shader.h"

static uint32_t screen_vao;
static uint32_t screen_vbo;
static NCXShader screen_shader;

void ncx_screen_buffer_create_buffers() {
	const float screen_vertices[] = {
		 1.0f, -1.0f,	1.0f, 0.0f,
		-1.0f,  1.0f,	0.0f, 1.0f,
		-1.0f, -1.0f,	0.0f, 0.0f,
	
		 1.0f, -1.0f,	1.0f, 0.0f,
		 1.0f,  1.0f,	1.0f, 1.0f,
		-1.0f,  1.0f,	0.0f, 1.0f,
	};

	glGenVertexArrays(1, &screen_vao);
	glBindVertexArray(screen_vao);

	glGenBuffers(1, &screen_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, screen_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screen_vertices), screen_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ncx_screen_buffer_create_shader() {
	screen_shader = ncx_shader_create("res/shaders/screen_vert.glsl", "res/shaders/screen_frag.glsl");
}

void ncx_screen_buffer_create(NCXScreenBuffer *buffer, const int32_t width, const int32_t height) {
	glGenFramebuffers(1, &buffer->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer->fbo);

	glGenTextures(1, &buffer->texture);
	glBindTexture(GL_TEXTURE_2D, buffer->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, buffer->texture, 0);

	glGenRenderbuffers(1, &buffer->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, buffer->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer->rbo);

	#ifdef DEBUG
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			printf("ERROR: Framebuffer fucked up\n");
			return;
		}
	#endif

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ncx_screen_buffer_draw(const NCXScreenBuffer buffer, const NCXTexture overlay, const float time, const float trip_intensity) {
	glDisable(GL_DEPTH_TEST);
	glUseProgram(screen_shader);
	glUniform1i(glGetUniformLocation(screen_shader, "screen_texture"), 0);
	glUniform1i(glGetUniformLocation(screen_shader, "trippy_texture"), 1);
	glUniform1i(glGetUniformLocation(screen_shader, "use_trippy_effect"), (overlay > 0));
	glUniform1f(glGetUniformLocation(screen_shader, "time"), time);
	glUniform1f(glGetUniformLocation(screen_shader, "trip_intensity"), trip_intensity);

	glBindVertexArray(screen_vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, buffer.texture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, overlay);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}

void ncx_screen_buffer_destroy(NCXScreenBuffer *buffer) {
	glDeleteRenderbuffers(1, &buffer->rbo);
	glDeleteTextures(1, &buffer->texture);
	glDeleteFramebuffers(1, &buffer->fbo);
}

void ncx_screen_buffer_destroy_shader() {
	glDeleteProgram(screen_shader);
}

void ncx_screen_buffer_destroy_buffers() {
	glDeleteBuffers(1, &screen_vbo);
	glDeleteVertexArrays(1, &screen_vao);
}

#include "narcotix/ui.h"
#include "narcotix/shader.h"

#include <string.h>

#include "narcotix/glad/glad.h"
#include "narcotix/cglm/cglm.h"

static mat4 projection;
static uint32_t vao, vbo;
static NCXShader ui_shader;

void ncx_ui_elements_init(const float width, const float height) {
	const float vertices[] = {
		0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 1.0f,

		0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 0.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	ui_shader = ncx_shader_create("res/shaders/ui_vert.glsl", "res/shaders/ui_frag.glsl", NULL);
	glUseProgram(ui_shader);
	glm_ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f, projection);
	glUniformMatrix4fv(glGetUniformLocation(ui_shader, "projection"), 1, GL_FALSE, (const float *)projection);
}

void ncx_ui_elements_set_flash(const float flash) {
	glUseProgram(ui_shader);
	glUniform1f(glGetUniformLocation(ui_shader, "flash_amount"), flash);
}

void ncx_ui_element_create(NCXUIElement *ncx_ui_element, float *pos, float *size, const NCXTexture *textures, const uint8_t texture_count) {
	glm_vec2_copy(pos, ncx_ui_element->pos);
	glm_vec2_copy(size, ncx_ui_element->size);
	ncx_ui_element->textures = calloc(texture_count, sizeof(NCXTexture));
	memcpy(ncx_ui_element->textures, textures, texture_count * sizeof(NCXTexture));
}

void ncx_ui_element_draw(const NCXUIElement ncx_ui_element, const uint8_t texture_index) {
	mat4 model;
	glDisable(GL_DEPTH_TEST);
	glm_mat4_identity(model);
	glm_translate(model, (vec3){ncx_ui_element.pos[0], ncx_ui_element.pos[1], 0.0f});
	glm_scale(model, (vec3){ncx_ui_element.size[0], ncx_ui_element.size[1], 1.0f});
	
	glBindVertexArray(vao);
	glUseProgram(ui_shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ncx_ui_element.textures[texture_index]);
	glUniformMatrix4fv(glGetUniformLocation(ui_shader, "model"), 1, GL_FALSE, (const float *)model);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}

void ncx_ui_element_destroy(NCXUIElement *ncx_ui_element) {
	free(ncx_ui_element->textures);
}

void ncx_ui_elements_terminate(void) {
	glDeleteProgram(ui_shader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

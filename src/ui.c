#include "narcotix/ui.h"
#include "narcotix/shader.h"
#include "narcotix/glad/glad.h"
#include <cglm/cglm.h>
#include <string.h>

static mat4 projection;
static uint32_t vao, vbo;
static ncx_shader_t ui_shader;

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

	glm_ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f, projection);

	ui_shader = ncx_shader_create("res/shaders/builtin/ui_vert.glsl", NULL,
			"res/shaders/builtin/ui_frag.glsl");
	ncx_shader_use(ui_shader);
	ncx_shader_uniform_mat4(ui_shader, "projection", projection);
}

void ncx_ui_elements_set_flash(const float flash) {
	ncx_shader_use(ui_shader);
	ncx_shader_uniform_float(ui_shader, "flash_amount", flash);
}

ncx_ui_element_t ncx_ui_element_create(float *pos, float *size,
		const ncx_texture_t *textures, const uint8_t texture_count) {

	ncx_ui_element_t element;
	element.textures = malloc(texture_count * sizeof(ncx_texture_t));
	memcpy(element.textures, textures, texture_count * sizeof(ncx_texture_t));
	memcpy(element.pos, pos, sizeof(vec2));
	memcpy(element.size, size, sizeof(vec2));

	return element;
}

void ncx_ui_element_draw(const ncx_ui_element_t element,
		const uint8_t index) {
	mat4 model;
	glDisable(GL_DEPTH_TEST);
	glm_mat4_identity(model);
	glm_translate(model, (vec3){element.pos[0], element.pos[1], 0.0f});
	glm_scale(model, (vec3){element.size[0], element.size[1], 1.0f});
	
	glBindVertexArray(vao);
	ncx_texture_use(element.textures[index], 0);

	ncx_shader_use(ui_shader);
	ncx_shader_uniform_mat4(ui_shader, "model", model);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}

void ncx_ui_elements_terminate(void) {
	glDeleteProgram(ui_shader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

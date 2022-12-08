#include "narcotix/ui.h"
#include "narcotix/shader.h"
#include "narcotix/glad/glad.h"

#include <string.h>
#include <malloc.h>

static uint32_t vao, vbo;
static ncx_shader_t ui_shader;

void ncx_ui_elems_init(const float width, const float height)
{
	const float verts[] = {
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

	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
			4 * sizeof(*verts), NULL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	ui_shader = ncx_shader_create("res/shaders/builtin/ui_vert.glsl", NULL,
			"res/shaders/builtin/ui_frag.glsl");

	ncx_shader_use(ui_shader);
	ncx_shader_uniform_mat4(ui_shader, "projection",
			ncx_mat4_ortho(0, width, height, 0));
}

void ncx_ui_elems_set_flash(const float flash)
{
	ncx_shader_use(ui_shader);
	ncx_shader_uniform_float(ui_shader, "flash_amount", flash);
}

struct ncx_ui_elem_t ncx_ui_elem_create(struct ncx_vec2 pos,
		struct ncx_vec2 size, const ncx_tex_t *tex,
		const uint8_t tex_count)
{
	struct ncx_ui_elem_t el = {
		.pos = pos,
		.size = size,
		.tex = malloc(tex_count * sizeof(ncx_tex_t)),
		.tex_count = tex_count,
	};

	memcpy(el.tex, tex, tex_count * sizeof(ncx_tex_t));

	return el;
}

void ncx_ui_elem_draw(const struct ncx_ui_elem_t el, const uint8_t index)
{
	struct ncx_mat4 model = NCX_MAT4_ID;
	glDisable(GL_DEPTH_TEST);
	model = ncx_mat4_trans(model, ncx_vec3(el.pos.x, el.pos.y, 0.0f));
	model = ncx_mat4_scale(model, ncx_vec3(el.size.x, el.size.y, 1.0f));
	
	glBindVertexArray(vao);
	ncx_tex_use(el.tex[index], 0);

	ncx_shader_use(ui_shader);
	ncx_shader_uniform_mat4(ui_shader, "model", model);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}

void ncx_ui_elems_terminate(void)
{
	glDeleteProgram(ui_shader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

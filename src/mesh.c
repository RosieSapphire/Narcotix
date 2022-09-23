#include "narcotix/mesh.h"

#include <string.h>
#include "narcotix/glad/glad.h"

ncx_mesh_t ncx_mesh_create(ncx_vertex_t *vertices, uint32_t *indices, ncx_texture_t *textures, const uint32_t vertex_count, const uint32_t index_count) {
	ncx_mesh_t m;
	m.vertices = malloc(vertex_count * sizeof(ncx_vertex_t));
	m.indices = malloc(index_count * sizeof(uint32_t));
	m.textures = malloc(2 * sizeof(ncx_texture_t));
	memcpy(m.vertices, vertices, vertex_count * sizeof(ncx_vertex_t));
	memcpy(m.indices, indices, index_count * sizeof(uint32_t));
	memcpy(m.textures, textures, 2 * sizeof(ncx_texture_t));

	glGenVertexArrays(1, &m.vao);
	glBindVertexArray(m.vao);

	glGenBuffers(1, &m.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(ncx_vertex_t), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(uint32_t), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, 	GL_FALSE, sizeof(ncx_vertex_t), NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, 	GL_FALSE, sizeof(ncx_vertex_t), (void *)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, 	GL_FALSE, sizeof(ncx_vertex_t), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	m.vertex_count = vertex_count;
	m.index_count = index_count;

	return m;
}

void ncx_mesh_draw(ncx_mesh_t m, uint32_t s) {
	const char *texture_slots[2] = {
		"material.tex_diffuse",
		"material.tex_specular",
	};

	for(uint8_t i = 0; i < 2; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glUniform1i(glGetUniformLocation(s, texture_slots[i]), (int32_t)i);
		glBindTexture(GL_TEXTURE_2D, m.textures[i]);
	}

	glBindVertexArray(m.vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
	glDrawElements(GL_TRIANGLES, (int32_t)m.index_count, GL_UNSIGNED_INT, 0);
}

void ncx_mesh_destroy(ncx_mesh_t *m) {
	free(m->vertices);
	free(m->indices);
	free(m->textures);
}

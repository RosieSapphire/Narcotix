#include "narcotix/mesh.h"

#include <string.h>
#include "narcotix/glad/glad.h"

NCXMesh ncx_mesh_create(NCXVertex *vertices, uint32_t *indices, NCXMaterial material, const uint32_t vertex_count, const uint32_t index_count) {
	NCXMesh m;
	m.vertices = malloc(vertex_count * sizeof(NCXVertex));
	m.indices = malloc(index_count * sizeof(uint32_t));
	m.material = material;

	memcpy(m.vertices, vertices, vertex_count * sizeof(NCXVertex));
	memcpy(m.indices, indices, index_count * sizeof(uint32_t));

	glGenVertexArrays(1, &m.vao);
	glBindVertexArray(m.vao);

	glGenBuffers(1, &m.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
	glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(NCXVertex), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &m.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(uint32_t), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, 	GL_FALSE, sizeof(NCXVertex), NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, 	GL_FALSE, sizeof(NCXVertex), (void *)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, 	GL_FALSE, sizeof(NCXVertex), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	m.vertex_count = vertex_count;
	m.index_count = index_count;

	return m;
}

void ncx_mesh_draw(NCXMesh m, uint32_t s) {
	const char *texture_slots[M_COUNT] = {
		"material.tex_diffuse",
		"material.tex_specular",
		"material.tex_normal",
	};

	for(uint8_t i = 0; i < M_COUNT; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glUniform1i(glGetUniformLocation(s, texture_slots[i]), (int32_t)i);
		glBindTexture(GL_TEXTURE_2D, m.material.textures[i]);
	}

	glUniform1f(glGetUniformLocation(s, "material.shininess"), m.material.shininess);

	glBindVertexArray(m.vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
	glDrawElements(GL_TRIANGLES, (int32_t)m.index_count, GL_UNSIGNED_INT, 0);
}

void ncx_mesh_destroy(NCXMesh *m) {
	free(m->vertices);
	free(m->indices);
}

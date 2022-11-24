#include "narcotix/mesh.h"

#include <stdio.h>
#include <string.h>
#include "narcotix/glad/glad.h"
#include <malloc.h>

#ifdef DEBUG
	#include "narcotix/debug.h"
	#include "rose_petal.h"
#endif

NCXMesh ncx_mesh_create(NCXVertex *vertices, uint32_t *indices,
		NCXMaterial material, const uint32_t vertex_count,
		const uint32_t index_count) {
	NCXMesh m;
	m.vertices = malloc(vertex_count * sizeof(NCXVertex));
	memcpy(m.vertices, vertices, vertex_count * sizeof(NCXVertex));

	m.indices = malloc(index_count * sizeof(uint32_t));
	memcpy(m.indices, indices, index_count * sizeof(uint32_t));

	m.material = material;

	glGenVertexArrays(1, &m.buffers[0]);
	glBindVertexArray(m.buffers[0]);

	glGenBuffers(2, m.buffers + 1);
	glBindBuffer(GL_ARRAY_BUFFER, m.buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(NCXVertex), vertices,
			GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.buffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(uint32_t),
			indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(0, 3, GL_FLOAT, 	GL_FALSE, sizeof(NCXVertex),
			(void *)offsetof(NCXVertex, pos));
	glVertexAttribPointer(1, 3, GL_FLOAT, 	GL_FALSE, sizeof(NCXVertex),
			(void *)offsetof(NCXVertex, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, 	GL_FALSE, sizeof(NCXVertex),
			(void *)offsetof(NCXVertex, uv));
	glVertexAttribPointer(3, 3, GL_FLOAT, 	GL_FALSE, sizeof(NCXVertex),
			(void *)offsetof(NCXVertex, tangent));
	glVertexAttribPointer(4, 3, GL_FLOAT, 	GL_FALSE, sizeof(NCXVertex),
			(void *)offsetof(NCXVertex, bitangent));

	glBindVertexArray(0);

	m.vertex_count = vertex_count;
	m.index_count = index_count;

	return m;
}

NCXMesh ncx_mesh_read(const char *path, NCXMaterial mat) {
	FILE *file = fopen(path, "rb");
	#ifdef DEBUG
		if(!file) {
			fprintf(stderr, "%sNARCOTIX::MESH::READ: %sFailed to read mesh"
					" from file %s'%s'%s.%s\n", D_COLOR_RED, D_COLOR_YELLOW,
					D_COLOR_GREEN, path, D_COLOR_YELLOW, D_COLOR_DEFAULT);

			NCXMesh mesh;
			mesh.vertex_count = 0;
			mesh.index_count = 0;
			mesh.vertices = NULL;
			mesh.indices = NULL;
			return mesh;
		}
	#endif

	NCXMesh mesh;
	fread(&mesh.vertex_count, sizeof(uint32_t), 2, file);
	mesh.vertices = calloc(mesh.vertex_count, sizeof(NCXVertex));
	mesh.indices = calloc(mesh.index_count, sizeof(uint32_t));
	fread(mesh.vertices, sizeof(NCXVertex), mesh.vertex_count, file);
	fread(mesh.indices, sizeof(uint32_t), mesh.index_count, file);

	fclose(file);

	mesh.material = mat;

	glGenVertexArrays(1, &mesh.buffers[0]);
	glBindVertexArray(mesh.buffers[0]);

	glGenBuffers(2, mesh.buffers + 1);
	glBindBuffer(GL_ARRAY_BUFFER, mesh.buffers[1]);
	glBufferData(GL_ARRAY_BUFFER, mesh.vertex_count * sizeof(NCXVertex),
			mesh.vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.buffers[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.index_count * sizeof(uint32_t),
			mesh.indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(0, 3, GL_FLOAT, 	GL_FALSE, sizeof(NCXVertex),
			(void *)offsetof(NCXVertex, pos));
	glVertexAttribPointer(1, 3, GL_FLOAT, 	GL_FALSE, sizeof(NCXVertex),
			(void *)offsetof(NCXVertex, normal));
	glVertexAttribPointer(2, 2, GL_FLOAT, 	GL_FALSE, sizeof(NCXVertex),
			(void *)offsetof(NCXVertex, uv));
	glVertexAttribPointer(3, 3, GL_FLOAT, 	GL_FALSE, sizeof(NCXVertex),
			(void *)offsetof(NCXVertex, tangent));
	glVertexAttribPointer(4, 3, GL_FLOAT, 	GL_FALSE, sizeof(NCXVertex),
			(void *)offsetof(NCXVertex, bitangent));

	glBindVertexArray(0);

	return mesh;
}

void ncx_mesh_write(NCXMesh *m, const char *path) {
	{
		FILE *tmp = fopen(path, "wb");
		if(tmp) {
			fclose(tmp);
			remove(path);
		} else {
			fclose(tmp);
		}
	}

	FILE *file = fopen(path, "wb");

	fwrite(&m->vertex_count, sizeof(uint32_t), 2, file);
	fwrite(m->vertices, sizeof(NCXVertex), m->vertex_count, file);
	fwrite(m->indices, sizeof(uint32_t), m->index_count, file);
	fclose(file);
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

	glUniform1f(glGetUniformLocation(s, "material.shininess"),
			m.material.shininess);

	glBindVertexArray(m.buffers[0]);
	glDrawElements(GL_TRIANGLES, (int32_t)m.index_count, GL_UNSIGNED_INT, 0);
}

void ncx_mesh_destroy(NCXMesh *m) {
	free(m->vertices);
	free(m->indices);
}

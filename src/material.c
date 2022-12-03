#include "narcotix/material.h"
#include "narcotix/glad/glad.h"
#include <malloc.h>

ncx_material_t ncx_material_create(const ncx_material_data_t data) {
	return (ncx_material_t) {
		data,
		ncx_texture_create(data.diffuse_path, GL_REPEAT,
				GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1),
		ncx_texture_create(data.specular_path, GL_REPEAT,
				GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1),
		ncx_texture_create(data.normal_path, GL_REPEAT,
				GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1),
	};
}

ncx_material_t *ncx_materials_create(const ncx_material_data_t *data,
		const uint32_t mat_count) {

	ncx_material_t *materials = malloc(mat_count * sizeof(ncx_material_t));
	for(uint32_t i = 0; i < mat_count; i++) {
		materials[i] = ncx_material_create(data[i]);
	}

	return materials;
}

void ncx_material_destroy(ncx_material_t *mat) {
	ncx_textures_destroy(&mat->diffuse, M_TEX_COUNT);
}

void ncx_materials_destroy(ncx_material_t *mat_start,
		const uint32_t mat_count) {
	const ncx_material_t *const mat_end = mat_start + mat_count;
	for(ncx_material_t *i = mat_start; i != mat_end; i++) {
		ncx_material_destroy(i);
	}
}

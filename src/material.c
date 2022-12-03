#include "narcotix/material.h"
#include "narcotix/glad/glad.h"
#include <malloc.h>

NCXMaterial ncx_material_create(const NCXMaterialData data) {
	return (NCXMaterial) {
		data,
		ncx_texture_create(data.diffuse_path, GL_REPEAT,
				GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1),
		ncx_texture_create(data.specular_path, GL_REPEAT,
				GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1),
		ncx_texture_create(data.normal_path, GL_REPEAT,
				GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1),
	};
}

NCXMaterial *ncx_materials_create(const NCXMaterialData *data,
		const uint32_t mat_count) {

	NCXMaterial *materials = malloc(mat_count * sizeof(NCXMaterial));
	for(uint32_t i = 0; i < mat_count; i++) {
		materials[i] = ncx_material_create(data[i]);
	}

	return materials;
}

void ncx_material_destroy(NCXMaterial *mat) {
	ncx_textures_destroy(&mat->diffuse, M_TEX_COUNT);
}

void ncx_materials_destroy(NCXMaterial *mat_start, const uint32_t mat_count) {
	const NCXMaterial *const mat_end = mat_start + mat_count;
	for(NCXMaterial *i = mat_start; i != mat_end; i++) {
		ncx_material_destroy(i);
	}
}

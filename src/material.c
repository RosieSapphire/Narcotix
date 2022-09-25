#include "narcotix/material.h"
#include "narcotix/glad/glad.h"

NCXMaterial ncx_material_create(const char **texture_paths, const float shininess) {
	NCXMaterial mat;
	for(uint8_t i = 0; i < 2; i++)
		mat.textures[i] = ncx_texture_create(texture_paths[i], GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1);

	mat.shininess = shininess;
	return mat;
}

void ncx_material_destroy(NCXMaterial mat) {
	ncx_textures_destroy(mat.textures, 2);
}

void ncx_materials_destroy(NCXMaterial* mats, const uint8_t mat_count) {
	for(uint8_t i = 0; i < mat_count; i++) {
		ncx_material_destroy(mats[i]);
	}
}

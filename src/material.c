#include "narcotix/material.h"
#include "narcotix/glad/glad.h"

#ifdef DEBUG
	#include <stdio.h>
	#include "narcotix/debug.h"
#endif

NCXMaterial ncx_material_create_internal(const char **texture_paths,
		const float shininess, const char *file, const uint32_t line) {
	NCXMaterial mat;
	for(uint8_t i = 0; i < M_COUNT; i++)
		mat.textures[i] = ncx_texture_create_internal(texture_paths[i],
				GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1, file, line);

	mat.shininess = shininess;
	#ifdef DEBUG
		printf("%sNARCOTIX::MATERIAL::CREATE: %sSuccessfully created material"
			"with shininess level %.0f. %s(Caused at '%s' line %u)\n",
			D_COLOR_GREEN, D_COLOR_YELLOW, shininess, D_COLOR_DEFAULT,
			file, line);
	#endif
	return mat;
}

void ncx_material_destroy(NCXMaterial mat) {
	ncx_textures_destroy(mat.textures, M_COUNT);
}

void ncx_materials_destroy(NCXMaterial* mats, const uint8_t mat_count) {
	for(uint8_t i = 0; i < mat_count; i++) {
		ncx_material_destroy(mats[i]);
	}
}

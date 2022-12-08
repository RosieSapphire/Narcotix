#include "narcotix/material.h"
#include "narcotix/glad/glad.h"
#include <malloc.h>

struct ncx_material ncx_material_create(const struct ncx_material_data data)
{
	return (struct ncx_material) {
		data,
		ncx_tex_create(data.diff_path, GL_REPEAT,
				GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1),
		ncx_tex_create(data.spec_path, GL_REPEAT,
				GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1),
		ncx_tex_create(data.norm_path, GL_REPEAT,
				GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR, 1),
	};
}

struct ncx_material *ncx_materials_create(const struct ncx_material_data *data,
		const uint32_t mat_count)
{
	struct ncx_material *materials =
		malloc(mat_count * sizeof(struct ncx_material));

	for(uint32_t i = 0; i < mat_count; i++) {
		materials[i] = ncx_material_create(data[i]);
	}

	return materials;
}

void ncx_material_destroy(struct ncx_material *mat)
{
	ncx_tex_destroy(&mat->diff, M_TEX_CNT);
}

void ncx_materials_destroy(struct ncx_material *mat_start,
		const uint32_t mat_count)
{
	const struct ncx_material *const mat_end = mat_start + mat_count;
	for(struct ncx_material *i = mat_start; i != mat_end; i++) {
		ncx_material_destroy(i);
	}
}

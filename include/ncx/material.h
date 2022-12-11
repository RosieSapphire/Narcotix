#ifndef __NCX_MATRIAL_H_
#define __NCX_MATRIAL_H_

#include "ncx/tex.h"

enum {
	M_TEX_DIFF = 0,
	M_TEX_SPEC,
	M_TEX_NORM,
	M_TEX_CNT
};

struct ncx_material_data {
	const char *diff_path, *spec_path, *norm_path;
	float shininess;
};

struct ncx_material {
	struct ncx_material_data data;
	ncx_tex_t diff, spec, norm;
};

struct ncx_material ncx_material_create(const struct ncx_material_data data);

struct ncx_material *ncx_materials_create(const struct ncx_material_data *data,
		const uint32_t mat_count);

void ncx_material_destroy(struct ncx_material *mat);

void ncx_materials_destroy(struct ncx_material *mat_start,
		const uint32_t mat_count);

#endif

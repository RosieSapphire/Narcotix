#ifndef __NCX_MATRIAL_H_
#define __NCX_MATRIAL_H_

#include "texture.h"

enum {
	M_TEX_DIFFUSE = 0,
	M_TEX_SPECULAR,
	M_TEX_NORMAL,
	M_TEX_COUNT
};

typedef struct {
	const char *diffuse_path, *specular_path, *normal_path;
	float shininess;
} ncx_material_data_t;

typedef struct {
	ncx_material_data_t data;
	ncx_texture_t diffuse, specular, normal;
} ncx_material_t;

ncx_material_t ncx_material_create(const ncx_material_data_t data);
ncx_material_t *ncx_materials_create(const ncx_material_data_t *data,
		const uint32_t mat_count);
void ncx_material_destroy(ncx_material_t *mat);
void ncx_materials_destroy(ncx_material_t *mat_start,
		const uint32_t mat_count);

#endif

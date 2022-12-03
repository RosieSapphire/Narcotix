#ifndef NCX_MATRIAL_H
#define NCX_MATRIAL_H

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
} NCXMaterialData;

typedef struct {
	NCXMaterialData data;
	NCXTexture diffuse, specular, normal;
} NCXMaterial;

NCXMaterial ncx_material_create(const NCXMaterialData data);
NCXMaterial *ncx_materials_create(const NCXMaterialData *data,
		const uint32_t mat_count);
void ncx_material_destroy(NCXMaterial *mat);
void ncx_materials_destroy(NCXMaterial *mat_start, const uint32_t mat_count);

#endif

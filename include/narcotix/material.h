#ifndef NCX_MATRIAL_H
#define NCX_MATRIAL_H

#include "texture.h"

enum {
	M_DIFFUSE = 0,
	M_SPECULAR,
	M_COUNT
};

typedef struct {
	NCXTexture textures[M_COUNT];
	float shininess;
} NCXMaterial;

NCXMaterial ncx_material_create(const char **texture_paths, const float shininess);
void ncx_material_destroy(NCXMaterial mat);
void ncx_materials_destroy(NCXMaterial* mats, const uint8_t mat_count);

#endif

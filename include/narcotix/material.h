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

NCXMaterial ncx_material_create_internal(const char **texture_paths, const float shininess, const char *file, const uint32_t line);
#define ncx_material_create(TEXTURE_PATHS, SHININESS) ncx_material_create_internal(TEXTURE_PATHS, SHININESS, __FILE__, __LINE__)
void ncx_material_destroy(NCXMaterial mat);
void ncx_materials_destroy(NCXMaterial* mats, const uint8_t mat_count);

#endif

#include "narcotix/texture.h"
#include "narcotix/glad/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "narcotix/stb_image.h"

ncx_texture_t ncx_texture_create(const char *path, const int32_t wrap_mode,
		const int32_t min_filter, const int32_t mag_filter,
		const uint8_t mipmap) {

	const int32_t color_formats[5] = { 0, GL_RED, GL_RG, GL_RGB, GL_RGBA };
	int32_t width, height, channels;
	ncx_texture_t t;

	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

	stbi_set_flip_vertically_on_load(1);
	uint8_t *data = stbi_load(path, &width, &height, &channels, 0);
	assert(data);
	glTexImage2D(GL_TEXTURE_2D, 0, color_formats[channels], width, height,
			0, color_formats[channels], GL_UNSIGNED_BYTE, data);
	stbi_image_free(data);

	if(mipmap) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	return t;
}

void ncx_textures_destroy(ncx_texture_t *start, const uint8_t texture_count) {
	glDeleteTextures(texture_count, start);
}

void ncx_texture_use(ncx_texture_t tex, const uint32_t slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, tex);
}

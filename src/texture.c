#include "narcotix/texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "narcotix/stb_image.h"

#include "narcotix/glad/glad.h"
#include <GLFW/glfw3.h>

#include "narcotix/debug.h"

NCXTexture ncx_texture_create_internal(const char *path, const int32_t wrap_mode, const int32_t min_filter, const int32_t mag_filter, const uint8_t mipmap, const char *file, const uint32_t line) {
	const int32_t color_formats[5] = { 0, GL_RED, GL_RG, GL_RGB, GL_RGBA };
	int32_t width, height, channels;
	NCXTexture t;

	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);

	stbi_set_flip_vertically_on_load(1);
	{
		uint8_t *data;
		data = stbi_load(path, &width, &height, &channels, 0);
		#ifdef DEBUG
			if(!data) {
				fprintf(stderr, "%sNARCOTIX::TEXTURE::ERROR: %sFucked up loading a texture from file: %s'%s'%s. %s(Caused at '%s' line %i)\n", D_COLOR_RED, D_COLOR_YELLOW, D_COLOR_GREEN, path, D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
				glfwTerminate();
				return t;
			} else {
				printf("%sNARCOTIX::TEXTURE::CREATE: %sSuccessfully loaded a texture from file: %s'%s'%s. %s(Caused at '%s' line %i)\n", D_COLOR_GREEN, D_COLOR_YELLOW, D_COLOR_GREEN, path, D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
			}
		#endif
		glTexImage2D(GL_TEXTURE_2D, 0, color_formats[channels], width, height, 0, (uint32_t)color_formats[channels], GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	if(mipmap)
		glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	return t;
}

void ncx_textures_destroy(NCXTexture *start, const uint8_t texture_count) {
	glDeleteTextures(texture_count, start);
}

void ncx_texture_use(NCXTexture tex, const uint32_t slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, tex);
}

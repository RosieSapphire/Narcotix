#include "narcotix/font_internal.h"

#include "narcotix/glad/glad.h"
#include "freetype/freetype.h"

#ifdef DEBUG
#include "narcotix/debug.h"
#endif

NCXShader ncx_font_shader_create_internal(const char *font_path_vert,
		const char *font_path_frag, const char *file, const uint32_t line) {

	mat4 matrix_projection;
	glm_ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f, matrix_projection);
	NCXShader font_shader = ncx_shader_create_internal(font_path_vert, NULL,
			font_path_frag, file, line);
	glUseProgram(font_shader);
	glUniformMatrix4fv(glGetUniformLocation(font_shader, "projection"),
			1, GL_FALSE, (const float *)matrix_projection);
	return font_shader;
}

NCXFont ncx_font_create_internal(const char *path, const char *file,
		const uint32_t line) {
	FT_Library ft;
	FT_Face face;
	NCXFont font;
	font.characters = NULL;
	#ifdef DEBUG
		if(FT_Init_FreeType(&ft)) {
			fprintf(stderr, "%sNARCOTIX::FONT::ERROR: %sFreetype Library "
					"initialization fucked up. %s(Caused at '%s' line %i)\n",
					D_COLOR_RED, D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
			return font;
		}

		if(FT_New_Face(ft, path, 0, &face)) {
			fprintf(stderr, "%sNARCOTIX::FONT::ERROR: %sFreetype failed to "
					"load font from file %s'%s'%s. %s(Caused at '%s' line %i)\n"
					, D_COLOR_RED, D_COLOR_YELLOW,  D_COLOR_GREEN, path,
					D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
			return font;
		}
	#else
		FT_Init_FreeType(&ft);
		FT_New_Face(ft, path, 0, &face);
	#endif

	FT_Set_Pixel_Sizes(face, 0, 72);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	font.characters = malloc(128 * sizeof(NCXCharacter));
	for(uint8_t c = 0; c < 128; c++) {
		if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			fprintf(stderr, "ERROR: Character '%c' loading fucked up\n", c);
			continue;
		}

		glGenTextures(1, &font.characters[c].texture);
		glBindTexture(GL_TEXTURE_2D, font.characters[c].texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
				(int32_t)face->glyph->bitmap.width,
				(int32_t)face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glm_ivec2_copy((ivec2){(int32_t)face->glyph->bitmap.width,
				(int32_t)face->glyph->bitmap.rows}, font.characters[c].size);
		glm_ivec2_copy((ivec2){(int32_t)face->glyph->bitmap_left,
				(int32_t)face->glyph->bitmap_top}, font.characters[c].bearing);
		font.characters[c].advance = (uint32_t)face->glyph->advance.x;
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &font.vao);
	glBindVertexArray(font.vao);
	glGenBuffers(1, &font.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, font.vbo);
	glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return font;
}

void ncx_font_destroy_internal(NCXFont *ncx_font, const char *file,
		const uint32_t line) {
	for(uint8_t i = 0; i < 128; i++) {
		glDeleteTextures(1, &ncx_font->characters[i].texture);
	}
	free(ncx_font->characters);

	#ifdef DEBUG
	printf("%sNARCOTIX::FONT::DESTROY: %sSuccessfully destroyed font. "
			"%s(Caused at '%s', line %d)\n", D_COLOR_GREEN, D_COLOR_YELLOW,
			D_COLOR_DEFAULT, file, line);
	#endif
}

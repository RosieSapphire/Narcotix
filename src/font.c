#include "narcotix/font.h"
#include "narcotix/shader.h"

#include "narcotix/cglm/cglm.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#include "narcotix/glad/glad.h"

#ifdef DEBUG
	#include "narcotix/debug.h"
#endif

static ncx_shader_t font_shader;

void ncx_font_shader_create(const char *font_path_vert, const char *font_path_frag) {
	mat4 matrix_projection;
	glm_ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f, matrix_projection);
	font_shader = ncx_shader_create(font_path_vert, font_path_frag);
	glUseProgram(font_shader);
	glUniformMatrix4fv(glGetUniformLocation(font_shader, "projection"), 1, GL_FALSE, (const float *)matrix_projection);
}

ncx_font_t ncx_font_create(const char *path) {
	FT_Library ft;
	FT_Face face;
	ncx_font_t ncx_font;
	ncx_font.characters = NULL;
	#ifdef DEBUG
		if(FT_Init_FreeType(&ft)) {
			fprintf(stderr, "%sNARCOTIX::FONT::ERROR:%s\t\tFreetype Library initialization fucked up. %s(Caused at '%s' line %i)\n", D_COLOR_RED, D_COLOR_YELLOW, D_COLOR_DEFAULT, __FILE__, __LINE__);
			return ncx_font;
		}

		if(FT_New_Face(ft, path, 0, &face)) {
			fprintf(stderr, "%sNARCOTIX::FONT::ERROR:%s\tFreetype failed to load font from file '%s'. %s(Caused at '%s' line %i)\n", D_COLOR_RED, D_COLOR_YELLOW,  path, D_COLOR_DEFAULT, __FILE__, __LINE__);
			return ncx_font;
		}
	#else
		FT_Init_FreeType(&ft);
		FT_New_Face(ft, path, 0, &face);
	#endif

	FT_Set_Pixel_Sizes(face, 0, 48);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	ncx_font.characters = calloc(128, sizeof(ncx_character_t));
	for(uint8_t c = 0; c < 128; c++) {
		if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			fprintf(stderr, "ERROR: Character '%c' loading fucked up\n", c);
			continue;
		}

		glGenTextures(1, &ncx_font.characters[c].texture);
		glBindTexture(GL_TEXTURE_2D, ncx_font.characters[c].texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (int32_t)face->glyph->bitmap.width, (int32_t)face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glm_ivec2_copy((ivec2){(int32_t)face->glyph->bitmap.width, (int32_t)face->glyph->bitmap.rows}, ncx_font.characters[c].size);
		glm_ivec2_copy((ivec2){(int32_t)face->glyph->bitmap_left, (int32_t)face->glyph->bitmap_top}, ncx_font.characters[c].bearing);
		ncx_font.characters[c].advance = (uint32_t)face->glyph->advance.x;
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &ncx_font.vao);
	glBindVertexArray(ncx_font.vao);
	glGenBuffers(1, &ncx_font.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, ncx_font.vbo);
	glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return ncx_font;
}

void ncx_font_draw(const ncx_font_t ncx_font, const char *string, float *pos, const float *color, const float scale, float *window_size) {
	const char *string_pointer;
	vec2 pos_scaled;
	pos_scaled[0] = pos[0] * (window_size[0] * (1920.0f / window_size[0]));
	pos_scaled[1] = pos[1] * (window_size[1] * (1080.0f / window_size[1]));

	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(font_shader);
	glUniform1i(glGetUniformLocation(font_shader, "text"), 0);
	glUniform3fv(glGetUniformLocation(font_shader, "text_color"), 1, color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(ncx_font.vao);
	for(string_pointer = string; *string_pointer; string_pointer++) {
		const ncx_character_t char_current = ncx_font.characters[(uint8_t)*string_pointer];
		const vec2 char_pos = {pos_scaled[0] + (float)char_current.bearing[0] * scale, pos_scaled[1] - (float)(char_current.size[1] - char_current.bearing[1]) * scale};
		const vec2 char_size = {(float)char_current.size[0] * scale, (float)char_current.size[1] * scale};
		const float vertices[6][4] = {
			{char_pos[0], 					char_pos[1] + char_size[1], 	0.0f, 0.0f},
			{char_pos[0], 					char_pos[1], 					0.0f, 1.0f},
			{char_pos[0] + char_size[0], 	char_pos[1], 					1.0f, 1.0f},

			{char_pos[0], 					char_pos[1] + char_size[1],		0.0f, 0.0f},
			{char_pos[0] + char_size[0],	char_pos[1],					1.0f, 1.0f},
			{char_pos[0] + char_size[0],	char_pos[1] + char_size[1],		1.0f, 0.0f},
		};

		glBindTexture(GL_TEXTURE_2D, char_current.texture);
		glBindBuffer(GL_ARRAY_BUFFER, ncx_font.vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);
		pos_scaled[0] += (float)(char_current.advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}

void ncx_font_destroy(ncx_font_t *ncx_font) {
	for(uint8_t i = 0; i < 128; i++) {
		glDeleteTextures(1, &ncx_font->characters[i].texture);
	}
	free(ncx_font->characters);
}

void ncx_font_shader_destroy() {
	glDeleteProgram(font_shader);
}


#include "ncx/font.h"
#include <freetype/freetype.h>
#include <assert.h>
#include <glad/gl.h>

ncx_shader_t ncx_font_shader_create(const char *vert, const char *frag)
{
	ncx_shader_t font_shader = ncx_shader_create(vert, NULL, frag);
	ncx_shader_use(font_shader);
	ncx_shader_uniform_mat4(font_shader, "proj",
			ncx_mat4_ortho(0, 1920, 1080, 0));

	return font_shader;
}

struct ncx_font ncx_font_create(const char *path)
{
	FT_Library ft;
	FT_Face face;
	struct ncx_font font;
	font.chars = NULL;

	if(FT_Init_FreeType(&ft)) {
		fprintf(stderr, "FONT ERROR: Failed to init Freetype.\n");
		assert(0);
	}

	if(FT_New_Face(ft, path, 0, &face)) {
		fprintf(stderr, "FONT ERROR: Couldn't create Freetype Face.\n");
		assert(0);
	}

	FT_Set_Pixel_Sizes(face, 0, 72);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	font.chars = malloc(128 * sizeof(struct ncx_char));
	for(uint8_t c = 0; c < 128; c++) {
		if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			fprintf(stderr, "ERROR: Character "
					"'%c' loading fucked up\n", c);
			continue;
		}

		struct ncx_char *char_cur = &font.chars[c];
		glGenTextures(1, &char_cur->texture);
		glBindTexture(GL_TEXTURE_2D, char_cur->texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
				(int32_t)face->glyph->bitmap.width,
				(int32_t)face->glyph->bitmap.rows,
				0, GL_RED, GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D,
				GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D,
				GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexParameteri(GL_TEXTURE_2D,
				GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D,
				GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		char_cur->size.x = face->glyph->bitmap.width;
		char_cur->size.y = face->glyph->bitmap.rows;
		char_cur->bearing.x = face->glyph->bitmap_left;
		char_cur->bearing.y = face->glyph->bitmap_top;
		char_cur->advance = face->glyph->advance.x;
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &font.vao);
	glBindVertexArray(font.vao);
	glGenBuffers(1, &font.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, font.vbo);

	glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(float),
			NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
			4 * sizeof(float), NULL);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return font;
}

void ncx_font_destroy(struct ncx_font *ncx_font)
{
	for(uint8_t i = 0; i < 128; i++) {
		glDeleteTextures(1, &ncx_font->chars[i].texture);
	}

	free(ncx_font->chars);
}

/**
 * @brief Draws a font with a specified string to the currently bound
 * 'ncx_render_buffer'
 *********************/
void ncx_font_draw(struct ncx_font font, const char *string,
		struct ncx_vec2 pos, struct ncx_vec3 color, float scale,
		struct ncx_vec2 window_size, ncx_shader_t shader)
{
	const char *string_pointer;
	struct ncx_vec2 pos_scaled;
	pos_scaled.x = pos.x * (window_size.x * (1920.0f / window_size.x));
	pos_scaled.y = pos.y * (window_size.y * (1080.0f / window_size.y));

	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ncx_shader_use(shader);
	ncx_shader_uniform_int(shader, "text", 0);
	ncx_shader_uniform_vec3(shader, "text_color", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(font.vao);
	for(string_pointer = string; *string_pointer; string_pointer++) {
		const struct ncx_char char_current =
			font.chars[(uint8_t)*string_pointer];

		const struct ncx_vec2 char_pos = {
			pos_scaled.x + char_current.bearing.x * scale,
			pos_scaled.y -
				(char_current.size.y - char_current.bearing.y)
				* scale
		};

		const struct ncx_vec2 char_size = {
			char_current.size.x * scale,
			(float)char_current.size.y * scale,
		};

		const float vertices[6][4] = {
			{char_pos.x, char_pos.y + char_size.y, 0.0f, 0.0f},
			{char_pos.x, char_pos.y, 0.0f, 1.0f},
			{char_pos.x + char_size.x, char_pos.y, 1.0f, 1.0f},

			{char_pos.x, char_pos.y + char_size.y, 0.0f, 0.0f},
			{char_pos.x + char_size.x, char_pos.y, 1.0f, 1.0f},
			{char_pos.x + char_size.x,
				char_pos.y + char_size.y, 1.0f, 0.0f},
		};

		glBindTexture(GL_TEXTURE_2D, char_current.texture);
		glBindBuffer(GL_ARRAY_BUFFER, font.vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);
		pos_scaled.x += (float)(char_current.advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}

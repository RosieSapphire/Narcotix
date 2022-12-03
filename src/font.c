#include "narcotix/glad/glad.h"
#include "narcotix/font.h"
#include <freetype/freetype.h>
#include <assert.h>

NCXShader ncx_font_shader_create(const char *font_path_vert,
		const char *font_path_frag) {

	mat4 matrix_projection;
	glm_ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f, matrix_projection);
	NCXShader font_shader = ncx_shader_create(font_path_vert, NULL,
			font_path_frag);
	ncx_shader_use(font_shader);
	ncx_shader_uniform_mat4(font_shader, "projection", matrix_projection);
	return font_shader;
}

NCXFont ncx_font_create(const char *path) {
	FT_Library ft;
	FT_Face face;
	NCXFont font;
	font.characters = NULL;

	uint32_t ft_init_status = FT_Init_FreeType(&ft);
	assert(!ft_init_status);
	uint32_t ft_new_face_status = FT_New_Face(ft, path, 0, &face);
	assert(!ft_new_face_status);

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

void ncx_font_destroy(NCXFont *ncx_font) {
	for(uint8_t i = 0; i < 128; i++) {
		glDeleteTextures(1, &ncx_font->characters[i].texture);
	}

	free(ncx_font->characters);
}

void ncx_font_draw(const NCXFont font, const char *string, float *pos,
		const float *color, const float scale, float *window_size,
		const NCXShader shader) {
	const char *string_pointer;
	vec2 pos_scaled;
	pos_scaled[0] = pos[0] * (window_size[0] * (1920.0f / window_size[0]));
	pos_scaled[1] = pos[1] * (window_size[1] * (1080.0f / window_size[1]));

	glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ncx_shader_use(shader);
	ncx_shader_uniform_int(shader, "text", 0);
	ncx_shader_uniform_vec3(shader, "text_color", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(font.vao);
	for(string_pointer = string; *string_pointer; string_pointer++) {
		const NCXCharacter char_current =
			font.characters[(uint8_t)*string_pointer];
		const vec2 char_pos = {pos_scaled[0]
			+ (float)char_current.bearing[0] * scale, pos_scaled[1]
				- (float)(char_current.size[1] - char_current.bearing[1])
				* scale};
		const vec2 char_size = {(float)char_current.size[0] * scale,
			(float)char_current.size[1] * scale};
		const float vertices[6][4] = {
			{char_pos[0], char_pos[1] + char_size[1], 0.0f, 0.0f},
			{char_pos[0], char_pos[1], 0.0f, 1.0f},
			{char_pos[0] + char_size[0], char_pos[1], 1.0f, 1.0f},

			{char_pos[0], char_pos[1] + char_size[1], 0.0f, 0.0f},
			{char_pos[0] + char_size[0], char_pos[1], 1.0f, 1.0f},
			{char_pos[0] + char_size[0], char_pos[1] + char_size[1],
				1.0f, 0.0f},
		};

		glBindTexture(GL_TEXTURE_2D, char_current.texture);
		glBindBuffer(GL_ARRAY_BUFFER, font.vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);
		pos_scaled[0] += (float)(char_current.advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}

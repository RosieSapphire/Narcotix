#include "narcotix/font.h"
#include "narcotix/shader.h"

#include <cglm/cglm.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "narcotix/glad/glad.h"

#ifdef DEBUG
	#include "narcotix/debug.h"
	// #include "rose_petal.h"
#endif

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

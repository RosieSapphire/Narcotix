#ifndef __NCX_CONTEXT_H_
#define __NCX_CONTEXT_H_

#include <cglm/cglm.h>
#include "narcotix/texture.h"

void ncx_init_internal(const float width, const float height,
		const uint8_t sbo_count, const char *window_name,
		const uint8_t use_blending, const char *file, const uint32_t line);
#define ncx_init(W, H, SBO_C, WN, UB) ncx_init_internal(W, H, SBO_C, WN, \
		UB, __FILE__, __LINE__)

float ncx_time_get(void);
void ncx_mouse_center(void);
uint8_t ncx_key_get(int32_t key);
uint8_t ncx_key_get_press(int32_t key);
uint8_t ncx_key_get_release(int32_t key);
uint8_t ncx_mouse_button_get(int32_t button);
void ncx_mouse_pos_get(vec2 mouse_pos);
void ncx_mouse_pos_set(vec2 mouse_pos);
void ncx_mouse_input_raw(const uint8_t toggle);
void ncx_clear_color(const float r, const float g,
		const float b, const float a);
void ncx_clear_depth(void);
void ncx_render_buffer_bind(const uint8_t index);
void ncx_render_buffer_unbind(void);
void ncx_buffer_display(const NCXTexture overlay, const float time,
		const float trip_intensity);
void ncx_buffer_swap(void);
void ncx_terminate(void);
uint8_t ncx_window_is_running(void);
void ncx_window_close(void);

#endif

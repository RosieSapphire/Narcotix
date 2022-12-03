#ifndef __NCX_CONTEXT_H_
#define __NCX_CONTEXT_H_

#include "narcotix/context_internal.h"
#include "narcotix/texture.h"

#define ncx_context_create(W, H, RB_C, WN, UB) \
	ncx_context_create_internal(W, H, RB_C, WN, UB, __FILE__, __LINE__)
#define ncx_context_destroy(CTX) \
	ncx_context_destroy_internal(CTX, __FILE__, __LINE__)

float ncx_context_time_get(void);
void ncx_context_time_delta_init(void);
float ncx_context_time_delta_get(void);
uint8_t ncx_context_key_get(const NCXContext context, const int32_t key);
uint8_t ncx_context_key_get_press(const NCXContext context, int32_t key);
uint8_t ncx_context_key_get_release(const NCXContext context, int32_t key);
uint8_t ncx_context_mouse_button_get(const NCXContext context, int32_t button);
void ncx_context_mouse_center(const NCXContext context);
void ncx_context_mouse_pos_get(const NCXContext context, vec2 mouse_pos);
void ncx_context_mouse_pos_set(const NCXContext context, vec2 mouse_pos);
void ncx_context_mouse_input_raw(const NCXContext context,
		const uint8_t toggle);
void ncx_context_clear_color(const float r, const float g,
		const float b, const float a);
void ncx_context_clear_depth(void);
void ncx_context_render_buffer_bind(const NCXContext context,
		const uint8_t index);
void ncx_context_render_buffer_unbind(void);
void ncx_context_buffer_display(const NCXContext context,
		const NCXTexture overlay, const float time,
		const float trip_intensity);
void ncx_context_buffer_swap(const NCXContext context);
uint8_t ncx_context_window_is_running(const NCXContext context);
void ncx_context_window_close(const NCXContext context);

#endif

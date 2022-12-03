#ifndef __NCX_RENDER_BUFFER_H_
#define __NCX_RENDER_BUFFER_H_

#include <stdint.h>
#include "narcotix/texture.h"

typedef struct {
	uint32_t fbo, rbo;
	ncx_texture_t texture;
} ncx_render_buffer_t;

#endif

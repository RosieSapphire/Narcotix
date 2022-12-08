#ifndef __NCX_RENDER_BUFFER_H_
#define __NCX_RENDER_BUFFER_H_

#include <stdint.h>
#include "narcotix/tex.h"

typedef struct {
	uint32_t fbo, rbo;
	ncx_tex_t tex;
} ncx_render_buffer_t;

#endif

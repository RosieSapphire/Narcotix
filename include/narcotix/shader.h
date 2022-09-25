#ifndef NCX_SHADER_H
#define NCX_SHADER_H

#include <stdint.h>

typedef uint32_t NCXShader;
NCXShader ncx_shader_create(const char *vert_path, const char *frag_path);

#endif

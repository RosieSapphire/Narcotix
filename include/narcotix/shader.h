#ifndef NCX_SHADER_H
#define NCX_SHADER_H

#include <stdint.h>

typedef uint32_t NCXShader;
NCXShader ncx_shader_create_internal(const char *vert_path, const char *geom_path, const char *frag_path, const char *file, const uint32_t line);
#define ncx_shader_create(VERT_PATH, FRAG_PATH, GEOM_PATH) ncx_shader_create_internal(VERT_PATH, FRAG_PATH, GEOM_PATH, __FILE__, __LINE__)

#endif

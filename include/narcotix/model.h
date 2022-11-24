#ifndef __NCX_MODEL_H_
#define __NCX_MODEL_H_

#include <assimp/scene.h>

#include "narcotix/mesh.h"
#include "narcotix/light_point.h"
#include <cglm/cglm.h>

NCXShader ncx_model_shader_create_internal(const NCXLightPoint *lights,
		const uint8_t light_count, const char *file, const uint32_t line);
#define ncx_model_shader_create(LIGHTS, LIGHT_COUNT) \
	ncx_model_shader_create_internal(LIGHTS, LIGHT_COUNT, __FILE__, __LINE__)

void ncx_model_shader_lights_update(const NCXShader shader,
		const NCXLightPoint *lights, const uint8_t light_count);

#endif

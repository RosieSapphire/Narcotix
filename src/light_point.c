#include "narcotix/light_point.h"
#include <string.h>
#include <cglm/cglm.h>

ncx_light_point_t ncx_light_point_create(float *pos, float *ambient_color,
		float *diffuse_color, float *specular_color, const float constant,
		const float linear, const float quadratic) {

	ncx_light_point_t light;
	memcpy(light.pos, pos, sizeof(vec3));
	memcpy(light.ambient_color, ambient_color, sizeof(vec3));
	memcpy(light.diffuse_color, diffuse_color, sizeof(vec3));
	memcpy(light.specular_color, specular_color, sizeof(vec3));
	light.constant = constant;
	light.linear = linear;
	light.quadratic = quadratic;

	return light;
}

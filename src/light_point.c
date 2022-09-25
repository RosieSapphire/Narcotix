#include "narcotix/light_point.h"
#include "narcotix/cglm/cglm.h"

NCXLightPoint ncx_light_point_create(float *pos, float *ambient_color, float *diffuse_color, float *specular_color, const float constant, const float linear, const float quadratic) {
	NCXLightPoint light;
	glm_vec3_copy(pos, light.pos);
	glm_vec3_copy(ambient_color, light.ambient_color);
	glm_vec3_copy(diffuse_color, light.diffuse_color);
	glm_vec3_copy(specular_color, light.specular_color);
	light.constant = constant;
	light.linear = linear;
	light.quadratic = quadratic;
	return light;
}

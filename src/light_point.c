#include "narcotix/light_point.h"
#include "narcotix/cglm/cglm.h"

#ifdef DEBUG
	#include "narcotix/debug.h"
#endif

NCXLightPoint ncx_light_point_create_internal(float *pos, float *ambient_color, float *diffuse_color, float *specular_color, const float constant, const float linear, const float quadratic, const char *file, const uint32_t line) {
	NCXLightPoint light;
	glm_vec3_copy(pos, light.pos);
	glm_vec3_copy(ambient_color, light.ambient_color);
	glm_vec3_copy(diffuse_color, light.diffuse_color);
	glm_vec3_copy(specular_color, light.specular_color);
	light.constant = constant;
	light.linear = linear;
	light.quadratic = quadratic;
	#ifdef DEBUG
		printf("%sNARCOTIX::LIGHT::POINT::CREATE: %sSuccessfully created point light at %s(%.2f, %.2f, %.2f)%s. %s(Caused at '%s' line %u)\n",
				D_COLOR_GREEN, D_COLOR_YELLOW, D_COLOR_GREEN, pos[0], pos[1], pos[2], D_COLOR_YELLOW, D_COLOR_DEFAULT, file, line);
	#endif
	return light;
}

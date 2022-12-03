#ifndef __NCX_LIGHT_POINT_H_
#define __NCX_LIGHT_POINT_H_

#include <stdint.h>

typedef struct {
	float pos[3];
	float ambient_color[3];
	float diffuse_color[3];
	float specular_color[3];
	float constant;
	float linear;
	float quadratic;
} NCXLightPoint;

NCXLightPoint ncx_light_point_create(float *pos,
		float *ambient_color, float *diffuse_color, float *specular_color,
		const float constant, const float linear, const float quadratic);

#endif

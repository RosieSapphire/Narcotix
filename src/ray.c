#include "ncx/ray.h"

uint8_t ncx_ray_triangle(struct ncx_vec3 eye,
		struct ncx_vec3 dir, struct ncx_vec3 *verts, float *distance)
{
	const float epsilon = 0.000001f;
	
	struct ncx_vec3 edge1 = ncx_vec3_sub(verts[1], verts[0]);
	struct ncx_vec3 edge2 = ncx_vec3_sub(verts[2], verts[0]);
	struct ncx_vec3 p = ncx_vec3_cross(dir, edge2);
	
	float det = ncx_vec3_dot(edge1, p);
	if(det > -epsilon && det < epsilon) {
		return 0;
	}
	
	float inv_det = 1.0f / det;
	struct ncx_vec3 t = ncx_vec3_sub(eye, verts[0]);
	float u = inv_det * ncx_vec3_dot(t, p);

	if(u < 0.0f || u > 1.0f) {
		return 0;
	}
	
	struct ncx_vec3 q = ncx_vec3_cross(t, edge1);
	float v = inv_det * ncx_vec3_dot(dir, q);

	if (v < 0.0f || u + v > 1.0f) {
		return 0;
	}
	
	float dist = inv_det * ncx_vec3_dot(edge2, q);
	*distance = dist;
	return dist > epsilon;
}

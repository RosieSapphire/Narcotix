#ifndef __NCX_VEC3_H_
#define __NCX_VEC3_H_

typedef struct {
	float x, y, z;
} ncx_vec3_t;

#define ncx_vec3(X, Y, Z) ((ncx_vec3_t){X, Y, Z})

#define ncx_vec3_0() ((ncx_vec3_t){0, 0, 0})
#define ncx_vec3_1() ((ncx_vec3_t){1, 1, 1})
#define ncx_vec3_x_up() ((ncx_vec3_t){1, 0, 0})
#define ncx_vec3_y_up() ((ncx_vec3_t){0, 1, 0})
#define ncx_vec3_z_up() ((ncx_vec3_t){0, 0, 1})

#define ncx_vec3_add(A, B) ((ncx_vec3_t){A.x + B.x, A.y + B.y, A.z + B.z})
#define ncx_vec3_sub(A, B) ((ncx_vec3_t){A.x - B.x, A.y - B.y, A.z - B.z})
#define ncx_vec3_scale(A, S) ((ncx_vec3_t){A.x * S, A.y * S, A.z * S})
#define ncx_vec3_cross(A, B) ((ncx_vec3_t){ A.y * B.z - A.z * B.y, \
		                                    A.z * B.x - A.x * B.z, \
											A.x * B.y - A.y * B.x,})

#endif

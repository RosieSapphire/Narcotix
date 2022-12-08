#ifndef __NCX_QUAT_H_
#define __NCX_QUAT_H_

#include "narcotix/vec3.h"
#include "narcotix/vec4.h"

#define ncx_quat_dot(A, B) (A.x * B.x + A.y * B.y + A.z * B.z + A.w * B.w)
ncx_vec4_t ncx_quat_lerp(ncx_vec4_t a, ncx_vec4_t b, float t);
ncx_vec4_t ncx_quat_slerp(ncx_vec4_t a, ncx_vec4_t b, float t);

#endif

#ifndef __NCX_QUAT_H_
#define __NCX_QUAT_H_

#include "ncx/vec3.h"
#include "ncx/vec4.h"

#define ncx_quat_dot(A, B) (A.x * B.x + A.y * B.y + A.z * B.z + A.w * B.w)
struct ncx_vec4 ncx_quat_lerp(struct ncx_vec4 a, struct ncx_vec4 b, float t);
struct ncx_vec4 ncx_quat_slerp(struct ncx_vec4 a, struct ncx_vec4 b, float t);

#endif

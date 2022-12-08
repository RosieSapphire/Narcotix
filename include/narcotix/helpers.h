#ifndef __NCX_HELPERS_H_
#define __NCX_HELPERS_H_

#include <stdint.h>

#define NCX_EPSILON	0.001f
#define NCX_PI      3.141592653589f
#define NCX_PI_INV  0.3183098861838710434f

#define NCX_TO_DEG  57.29577951309678781159f
#define NCX_TO_RAD  0.01745329251993888889f

float ncx_clampf(const float val, const float min, const float max);
float ncx_clampf01(const float val);
void  ncx_printfv(const float *vec, const uint8_t size);
void  ncx_printfm(const float **mat, const uint8_t size);
void  ncx_printfvi(const int32_t *vec, const uint8_t size);
int   ncx_sprintfv1(char *dest, const char *format, const float *vec);
int   ncx_sprintfv2(char *dest, const char *format, const float *vec);
int   ncx_sprintfv3(char *dest, const char *format, const float *vec);
float ncx_fmod(const float val, const float mod);
float ncx_lerp(float a, float b, float t);
float ncx_lerpc(float a, float b, float t);

void glfw_error_callback(int code, const char *log);

/*
 * This is a tidied up version of the Fast Inverse Square Root
 * function from Quake III Arena. Originally written by John Carmack
 */
float ncx_quake_rsqrt(float x);

#endif

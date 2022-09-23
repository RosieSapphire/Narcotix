#ifndef NCX_HELPERS_H
#define NCX_HELPERS_H

#include "narcotix/cglm/cglm.h"

float ncx_clampf(const float val, const float min, const float max);
float ncx_clampf01(const float val);
void  ncx_printfv(const float *vec, const uint8_t size);
void  ncx_printfm(const float **mat, const uint8_t size);
void  ncx_printfvi(const int32_t *vec, const uint8_t size);
int   ncx_sprintfv1(char *dest, const char *format, const float *vec);
int   ncx_sprintfv2(char *dest, const char *format, const float *vec);
int   ncx_sprintfv3(char *dest, const char *format, const float *vec);
float ncx_fmod(const float val, const float mod);

void  ncx_vec3_lerpc(vec3 a, vec3 b, const float t, vec3 out);
void  ncx_vec2_lerpc(vec2 a, vec2 b, const float t, vec2 out);
float ncx_lerpc(const float a, const float b, const float t);

#endif

#include "narcotix/helpers.h"

#include <stdio.h>

/* NOTICE: This code was made by OwenTheProgrammer, and can be found at
 * https://github.com/OwenTheProgrammer/DemoScripts */
float ncx_clampf(const float val, const float min, const float max)
{
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wstrict-aliasing"
	#pragma GCC diagnostic ignored "-Wuninitialized"
    const float diff[2] = {min - val, val - max};
	uint8_t flag_a = (*(uint32_t *)&diff[0] >> 31);
	uint8_t flag_b = (*(uint32_t *)&diff[1] >> 31);
    uint8_t flag_and = (flag_a & flag_b);
	#pragma GCC diagnostic pop
    return (float)((flag_and * val) + ((1 - flag_a) * min)
			+ ((1 - flag_b) * max));
}

float ncx_clampf01(const float val)
{
	return ncx_clampf(val, 0.0f, 1.0f);
}

void  ncx_printfv(const float *vec, const uint8_t size)
{
	for(uint8_t i = 0; i < size; i++) {
		printf("%f ", (const double)vec[i]);
	}
	printf("\n");
}

void  ncx_printfm(const float **mat, const uint8_t size)
{
	for(uint8_t i = 0; i < size; i++) {
		for(uint8_t j = 0; j < size; j++) {
			printf("%f ", (const double)mat[i][j]);
			printf("\n");
		}
	}
	printf("\n");
}

void  ncx_printfvi(const int32_t *vec, const uint8_t size)
{
	for(uint8_t i = 0; i < size; i++) {
		printf("%i ", vec[i]);
	}
	printf("\n");
}

int   ncx_sprintfv1(char *dest, const char *format, const float *vec)
{
	return sprintf(dest, format, *vec);
}

int   ncx_sprintfv2(char *dest, const char *format, const float *vec)
{
	return sprintf(dest, format, vec[0], vec[1]);
}

int   ncx_sprintfv3(char *dest, const char *format, const float *vec)
{
	return sprintf(dest, format, vec[0], vec[1], vec[2]);
}

float ncx_fmod(const float val, const float mod)
{
	const float val_scaled = val / mod;
	return (val_scaled - (float)((int32_t)val_scaled)) * mod;
}

float ncx_lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

float ncx_lerpc(float a, float b, float t)
{
	return ncx_lerp(a, b, ncx_clampf01(t));
}

void glfw_error_callback(int code, const char *log)
{
	printf("NARCOTIX::GLFW::ERROR: %s : %i\n", log, code);
}

float ncx_quake_rsqrt(float x)
{
	uint32_t x_bytes = *(uint32_t *)&x;
	x_bytes = 0x5f3759df - ( x_bytes >> 1 );

	float result = *(float *)&x_bytes;
	result *= 1.5f - (x * 0.5f * result * result);

	return result;
}

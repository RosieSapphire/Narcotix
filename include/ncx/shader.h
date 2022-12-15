#ifndef __NCX_SHADER_H_
#define __NCX_SHADER_H_

#include <stdint.h>
#include "ncx/vec3.h"
#include "ncx/mat4.h"

/**
 * @brief An identifier for an OpenGL shader,
 * which can be generated with 'ncx_shader_create()'
 ***************************************************/
typedef uint32_t ncx_shader_t;

/**
 * @brief Creates a new shader in the OpenGL context.
 * @param vert_path The path to the vertex shader source code.
 * @param geom_path The path to the geometry shader source code. (OPTIONAL)
 * @param frag_path The path to the fragment shader source code.
 ***************************************************************/
ncx_shader_t ncx_shader_create(const char *vert_path,
		const char *geom_path, const char *frag_path);

/**
 * @brief Destroys a shader from the OpenGL context.
 * @param shader The shader you want to destroy.
 */
void ncx_shader_destroy(const ncx_shader_t shader);

/**
 * @brief Binds a specific shader for use.
 * @param shader The shader you want to use/bind.
 */
void ncx_shader_use(const ncx_shader_t shader);

/**
 * @defgroup Shader Uniform Functions
 * @brief Functions for loading in different data to shaders as uniforms.
 * @{
 */

/**
 * @brief Upload an integer to a shader's uniform.
 * @param shader The shader you want to upload the uniform to.
 * @param uniform The name of the uniform you want to set.
 * @param value The value you want to set the specified uniform to.
 */
void ncx_shader_uniform_int(const ncx_shader_t shader, const char *uniform,
		const int value);

/**
 * @brief Upload a floating point number to a shader's uniform.
 * @param shader The shader you want to upload the uniform to.
 * @param uniform The name of the uniform you want to set.
 * @param value The value you want to set the specified uniform to.
 */
void ncx_shader_uniform_float(const ncx_shader_t shader, const char *uniform,
		const float value);

/**
 * @brief Upload an 'ncx_vec3' to a shader's uniform.
 * @param shader The shader you want to upload the uniform to.
 * @param uniform The name of the uniform you want to set.
 * @param value The value you want to set the specified uniform to.
 */
void ncx_shader_uniform_vec3(const ncx_shader_t shader, const char *uniform,
		const struct ncx_vec3 value);

/**
 * @brief Upload an 'ncx_mat4' to a shader's uniform.
 * @param shader The shader you want to upload the uniform to.
 * @param uniform The name of the uniform you want to set.
 * @param value The value you want to set the specified uniform to.
 */
void ncx_shader_uniform_mat4(const ncx_shader_t shader, const char *uniform,
		const struct ncx_mat4 value);
/**
 * @}
 */

#endif

#ifndef __NCX_CONTEXT_H_
#define __NCX_CONTEXT_H_

#include "ncx/shader.h"
#include "ncx/render_buffer.h"
#include "ncx/vec2.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

/**
 * Enum for each of the mouse buttons.
 ********************************************/
enum ncx_mouse_buttons {
	NCX_MB_LEFT,
	NCX_MB_RIGHT,
};

/**
 * Enum for each of the input states (pressed, held, released)
 ********************************************************************/
enum input_state {
	NCX_PRESS,
	NCX_HOLD,
	NCX_RELEASE,
};

/**
 * Initializes a GLFW context, and loads all OpenGL functions for use.
 * @param width Width of the window you want to create
 * @param height Height of the window you want to create
 * @param rb_count Amount of render buffers you want to create. They will
 * be drawn all at once with 'ncx_buffer_display()'.
 ***************************************************/
void ncx_init(const float width, const float height,
		const uint8_t rb_count, const char *window_name,
		const uint8_t use_blending);

/**
 * Terminates the GLFW context set up by 'ncx_init()'.
 *****************************************************/
void ncx_terminate(void);

/**
 * Returns the current time of the application's runtime.
 ********************************************************/
float ncx_time(void);

/**
 * Stores the current time in the "time_last" variable used for calculating
 * delta-time. This is so there's not a massive spike of speed right as the
 * application is booting up. Just make sure this is called before your
 * main loop so there's as little time as possible between when it gets
 * calculated next.
 ******************/
void ncx_time_delta_init(void);

/**
 * Returns the current calculated delta-time.
 *******************************************/
float ncx_time_delta(void);

/**
 * Returns whether a specified key is pressed, held, or released.
 * Uses the NCXK_... enum for which key you want, and NCX_INP_... for 
 * either PRESS, HOLD, or RELEASE.
 * @param key The desired key you want to test for.
 * @param state_wanted The specific state you want to check the key for.
 ***********************************************************************/
uint8_t ncx_key_get(const int32_t key, uint32_t state_wanted);

/**
 * Returns whether a specified mouse button is pressed, held, or released.
 * @param button The desired mouse button you want to test for.
 * @param state_wanted The specific state you want to check the button for.
 * @note Uses NCX_MB_LEFT and NCX_MB_RIGHT.
 ******************************************/
uint8_t ncx_mouse_button_get(int32_t button, uint32_t state_wanted);

/**
 * Centers the mouse in the window.
 **********************************/
void ncx_mouse_center(void);

/**
 * Returns the mouse's current position relative to the window.
 **************************************************************/
struct ncx_vec2 ncx_mouse_pos_get(void);

/**
 * Sets the mouse's position to a desired value.
 * @param new_pos The new pos you want the mouse to be moved to.
 ***************************************************************/
void ncx_mouse_pos_set(struct ncx_vec2 new_pos);

/**
 * @param toggle Whether you want raw mouse input on or off.
 ***********************************************************/
void ncx_mouse_input_raw(const uint8_t toggle);

/**
 * Clears the render view with a desired color.
 * @param r Red
 * @param g Green
 * @param b Blue
 * @param a Alpha
 ****************/
void ncx_clear_color(const float r, const float g,
		const float b, const float a);

/**
 * Clears the depth buffer.
 **************************/
void ncx_clear_depth(void);

/**
 * Binds a selected render buffer to draw on it.
 * @param The index of the render buffer you want to be bound.
 *************************************************************/
void ncx_render_buffer_bind(const uint8_t index);

/**
 * Unbinds and bound render buffers.
 ***********************************/
void ncx_render_buffer_unbind(void);

/**
 * Displays the render buffers at once.
 * @param overlay An optional texture to overlay the entire screen with.
 * @param time The current time the application has been running (in seconds).
 * @param trip_intensity COMPLETELY OPTIONAL, has to do with the overlay.
 ************************************************************************/
void ncx_buffer_display(const ncx_tex_t overlay, const float time,
		const float trip_intensity);

/**
 * Swap the front and back buffers to start working on the next frame.
 ********************************************************************/
void ncx_buffer_swap(void);

/**
 * Returns whether the window is currently running or not.
 *********************************************************/
uint8_t ncx_window_is_running(void);

/**
 * Mark the window for closing next time the loop runs.
 ******************************************************/
void ncx_window_close(void);

#endif

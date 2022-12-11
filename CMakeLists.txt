cmake_minimum_required(VERSION 3.13)
project(ncx)
add_library(ncx src/context.c src/shader.c src/gl.c
	src/material.c src/tex.c src/font.c src/mesh.c src/model.c src/mat4.c
	src/vec4.c src/vec3.c src/vec2.c src/helpers.c src/sound.c src/file.c
	src/light_point.c src/animation.c src/quat.c)
target_include_directories(ncx PRIVATE include
	/usr/include/freetype2/)
target_link_libraries(ncx PRIVATE glfw freetype openal sndfile
	assimp m)

add_executable(ncx_test src/test.c)
target_include_directories(ncx_test PRIVATE include)
target_link_libraries(ncx_test PRIVATE ncx)

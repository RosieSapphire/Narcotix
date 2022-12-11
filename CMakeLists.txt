cmake_minimum_required(VERSION 3.13)
project(ncx)
add_executable(${PROJECT_NAME} src/test.c src/context.c src/shader.c src/gl.c
	src/material.c src/tex.c src/font.c src/mesh.c src/model.c src/mat4.c
	src/vec4.c src/vec3.c src/vec2.c src/helpers.c src/sound.c src/file.c
	src/light_point.c src/animation.c src/quat.c)
target_include_directories(${PROJECT_NAME} PRIVATE include
	/usr/include/freetype2/)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw freetype openal sndfile
	assimp m)

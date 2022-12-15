cmake_minimum_required(VERSION 3.13)

set(RES_DIR "\"${CMAKE_SOURCE_DIR}/res/\"" CACHE STRING "The 'res' directory")
set(WIN_WIDTH "1280" CACHE STRING "The width of the window")
set(WIN_HEIGHT "720" CACHE STRING "The height of the window")
add_definitions(-DRES_DIR=${RES_DIR})
add_definitions(-DWIN_WIDTH=${WIN_WIDTH})
add_definitions(-DWIN_HEIGHT=${WIN_HEIGHT})

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

#ifndef __NCX_TEST_PATHS_H_
#define __NCX_TEST_PATHS_H_

#include "narcotix/material.h"

const char *font_shader_vert_path = "res/shaders/builtin/font_vert.glsl";
const char *font_shader_frag_path = "res/shaders/builtin/font_frag.glsl";
const char *trippy_font_path = "res/fonts/shagadelic.ttf";
const char *normal_font_path = "res/fonts/jetbrainsmono-bold.ttf";
const char *trippy_tex_path = "res/textures/trippy-overlay-texture.png";
const char *pistol_tex_paths[M_TEX_COUNT] = {
	"res/models/weapons/pistol/pistol_diffuse.png",
	"res/models/weapons/pistol/pistol_specular.png",
	"res/models/weapons/pistol/pistol_normal.png"
};

const char *pistol_model_path = "res/models/weapons/pistol/pistol.glb";
const char *brick_tex_paths[M_TEX_COUNT] = {
	"res/textures/bricks_diffuse.png",
	"res/textures/bricks_specular.png",
	"res/textures/bricks_normal.png",
};

const char *brick_model_path = "res/models/plane.glb";

#endif

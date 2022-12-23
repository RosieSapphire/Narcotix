#include "ncx/model.h"

#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#define POINT_LIGHT_MAX 32

ncx_shader_t ncx_model_shader_create(const struct ncx_light_point *lights,
		const uint8_t light_count)
{
	ncx_shader_t model_shader =
		ncx_shader_create(RES_DIR"shdr/internal/model_vert.glsl", NULL,
				RES_DIR"shdr/internal/model_frag.glsl");
	ncx_shader_use(model_shader);
	ncx_shader_uniform_int(model_shader, "light_points_count_current",
			light_count);

	/* uploading light properties to shader */
	for(uint8_t i = 0; i < light_count; i++) {
		uint8_t j;
		char buffer[128] = { 0 };
		const struct ncx_light_point light_cur = lights[i];
		const char *properties[7] = {
			"pos", "ambi_col", "diff_col", "spec_col",
			"constant", "linear", "quadratic"
		};

		const struct ncx_vec3 vectors[4] = {
			light_cur.pos, light_cur.ambi_col,
			light_cur.diff_col, light_cur.spec_col,
		};

		const float values[3] = {
			light_cur.constant,
			light_cur.linear,
			light_cur.quadratic
		};

		for(j = 0; j < 4; j++) {
			sprintf(buffer, "light_points[%u].%s",
					i, properties[j]);

			ncx_shader_uniform_vec3(model_shader,
					buffer, vectors[j]);
		}

		for(; j < 7; j++) {
			sprintf(buffer, "light_points[%u].%s",
					i, properties[j]);

			ncx_shader_uniform_float(model_shader,
					buffer, values[j - 4]);
		}
	}

	return model_shader;
}

void ncx_model_shader_lights_update(const ncx_shader_t shader,
		const struct ncx_light_point *lights,
		const uint8_t light_count)
{
	ncx_shader_use(shader);
	const char *properties[7] = {
		"pos", "ambi_col", "diff_col", "spec_col",
		"constant", "linear", "quadratic",
	};

	/* reset all lights to ensure no bullshittery */
	for(uint8_t i = 0; i < POINT_LIGHT_MAX; i++) {
		uint8_t j;
		char buffer[128] = { 0 };

		for(j = 0; j < 4; j++) {
			sprintf(buffer, "light_points[%u].%s",
					i, properties[j]);

			ncx_shader_uniform_vec3(shader, buffer, NCX_VEC3_0);
		}

		for(; j < 7; j++) {
			sprintf(buffer, "light_points[%u].%s",
					i, properties[j]);

			ncx_shader_uniform_float(shader, buffer, 0.0f);
		}
	}

	/* now import the new lights */
	for(uint8_t i = 0; i < light_count; i++) {
		uint8_t j;
		char buffer[128] = { 0 };
		const struct ncx_light_point light_cur = lights[i];
		const struct ncx_vec3 vectors[4] = {
			light_cur.pos, light_cur.ambi_col, light_cur.diff_col,
			light_cur.spec_col
		};

		const float values[3] = {
			light_cur.constant,
			light_cur.linear,
			light_cur.quadratic
		};

		for(j = 0; j < 4; j++) {
			sprintf(buffer, "light_points[%u].%s",
					i, properties[j]);

			ncx_shader_uniform_vec3(shader, buffer, vectors[j]);
		}

		for(; j < 7; j++) {
			sprintf(buffer, "light_points[%u].%s",
					i, properties[j]);

			ncx_shader_uniform_float(shader, buffer, values[j - 4]);
		}
	}

	ncx_shader_uniform_int(shader,
			"light_points_count_current", light_count);
}

struct ncx_model ncx_model_create(const char *path,
		const struct ncx_material *materials,
		const uint8_t is_animated)
{
	struct ncx_model model;
	model.scene = aiImportFile(path,
			aiProcess_Triangulate |
			aiProcess_OptimizeMeshes |
			aiProcess_JoinIdenticalVertices |
			aiProcess_CalcTangentSpace);

	if(!model.scene) {
		fprintf(stderr, "MODEL ERROR: Couldn't load model from '%s'\n",
				path);

		assert(0);
	}

	model.meshes =
		ncx_meshes_create(model.scene, materials, &model.mesh_count);

	model.anim_count = 0;
	model.anim_selected = 0;
	model.anims = NULL;
	if(is_animated) {
		model.anims =
			ncx_animations_create(model.scene, &model.anim_count);
	}

	return model;
}

void ncx_model_draw(struct ncx_model model,
		const ncx_shader_t shader, struct ncx_mat4 root)
{
	if(model.anims) {
		ncx_meshes_draw_anim(model.meshes, model.mesh_count, shader,
				model.anims[model.anim_selected], root);
	} else {
		ncx_meshes_draw(model.meshes, model.mesh_count, shader);
	}
}

void ncx_model_animation_set(struct ncx_model *model,
		const uint32_t anim_index)
{
	model->anim_selected = anim_index;
	model->anims[model->anim_selected].timer = 0.0f;
}

void ncx_model_animation_update(struct ncx_model *model, const float dt,
		const uint8_t loop)
{
	struct ncx_anim *anim = model->anims + model->anim_selected;
	anim->timer += dt * ANIM_FPS;

	if(loop) {
		anim->timer /= anim->channels->tick_count;
		anim->timer -= (int)anim->timer;
		anim->timer *= anim->channels->tick_count;
	} else {
		if(anim->timer >= anim->channels->tick_count - 1) {
			anim->timer = anim->channels->tick_count - 1;
		}
	}
}

void ncx_model_destroy(struct ncx_model *model)
{
	for(uint32_t i = 0; i < model->anim_count; i++) {
		for(uint32_t j = 0; j < model->anims[i].channel_count; j++) {
			free(model->anims[i].channels[j].pos_keys);
			free(model->anims[i].channels[j].quat_keys);
		}

		free(model->anims[i].channels);
	}
	free(model->anims);

	for(uint32_t i = 0; i < model->mesh_count; i++) {
		free(model->meshes[i].vertices);
		free(model->meshes[i].indices);
	}
}

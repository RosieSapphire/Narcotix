#include "narcotix/model.h"
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

#define POINT_LIGHT_MAX 32

NCXShader ncx_model_shader_create(const NCXLightPoint *lights,
		const uint8_t light_count) {

	NCXShader model_shader =
		ncx_shader_create("res/shaders/builtin/model_vert.glsl", NULL,
				"res/shaders/builtin/model_frag.glsl");
	ncx_shader_use(model_shader);
	ncx_shader_uniform_int(model_shader, "light_points_count_current",
			light_count);

	/* uploading light properties to shader */
	for(uint8_t i = 0; i < light_count; i++) {
		uint8_t j;
		char buffer[128] = { 0 };
		const NCXLightPoint light_cur = lights[i];
		const char *properties[7] = {
			"pos", "ambient_color", "diffuse_color", "specular_color",
			"constant", "linear", "quadratic"
		};

		const float *vectors[4] = {
			light_cur.pos, light_cur.ambient_color, light_cur.diffuse_color,
			light_cur.specular_color
		};

		const float values[3] = {
			light_cur.constant, light_cur.linear, light_cur.quadratic
		};

		for(j = 0; j < 4; j++) {
			sprintf(buffer, "light_points[%u].%s", i, properties[j]);
			ncx_shader_uniform_vec3(model_shader, buffer, vectors[j]);
		}

		for(; j < 7; j++) {
			sprintf(buffer, "light_points[%u].%s", i, properties[j]);
			ncx_shader_uniform_float(model_shader, buffer, values[j - 4]);
		}
	}

	return model_shader;
}

void ncx_model_shader_lights_update(const NCXShader shader,
		const NCXLightPoint *lights, const uint8_t light_count) {
	ncx_shader_use(shader);
	const char *properties[7] = {
		"pos", "ambient_color", "diffuse_color", "specular_color",
		"constant", "linear", "quadratic",
	};

	/* reset all lights to ensure no bullshittery */
	for(uint8_t i = 0; i < POINT_LIGHT_MAX; i++) {
		uint8_t j;
		char buffer[128] = { 0 };

		for(j = 0; j < 4; j++) {
			sprintf(buffer, "light_points[%u].%s", i, properties[j]);
			ncx_shader_uniform_vec3(shader, buffer, GLM_VEC3_ZERO);
		}

		for(; j < 7; j++) {
			sprintf(buffer, "light_points[%u].%s", i, properties[j]);
			ncx_shader_uniform_float(shader, buffer, 0.0f);
		}
	}

	/* now import the new lights */
	for(uint8_t i = 0; i < light_count; i++) {
		uint8_t j;
		char buffer[128] = { 0 };
		const NCXLightPoint light_cur = lights[i];
		const float *vectors[4] = {
			light_cur.pos, light_cur.ambient_color, light_cur.diffuse_color,
			light_cur.specular_color
		};

		const float values[3] = {
			light_cur.constant, light_cur.linear, light_cur.quadratic
		};

		for(j = 0; j < 4; j++) {
			sprintf(buffer, "light_points[%u].%s", i, properties[j]);
			ncx_shader_uniform_vec3(shader, buffer, vectors[j]);
		}

		for(; j < 7; j++) {
			sprintf(buffer, "light_points[%u].%s", i, properties[j]);
			ncx_shader_uniform_float(shader, buffer, values[j - 4]);
		}
	}

	ncx_shader_uniform_int(shader, "light_points_count_current", light_count);
}

NCXModel ncx_model_create(const char *path, const NCXMaterial *materials,
		const uint8_t is_animated) {
	NCXModel model;
	model.scene = aiImportFile(path, aiProcess_Triangulate |
			aiProcess_OptimizeMeshes | aiProcess_JoinIdenticalVertices |
			aiProcess_CalcTangentSpace);

	model.meshes =
		ncx_meshes_create(model.scene, materials, &model.mesh_count);

	model.anim_count = 0;
	model.anim_selected = 0;
	model.anims = NULL;
	if(is_animated) {
		model.anims = ncx_animations_create(model.scene, &model.anim_count);
	}

	return model;
}

void ncx_model_draw(NCXModel model, const NCXShader shader, mat4 root) {
	if(model.anims) {
		ncx_meshes_draw_anim(model.meshes, model.mesh_count, shader,
				model.anims[model.anim_selected], root);
	} else {
		ncx_meshes_draw(model.meshes, model.mesh_count, shader, root);
	}
}

void ncx_model_animation_set(NCXModel *model, const uint32_t anim_index) {
	model->anim_selected = anim_index;
	model->anims[model->anim_selected].timer = 0.0f;
}

void ncx_model_animation_update(NCXModel *model, const float dt,
		const uint8_t loop) {
	NCXAnimation *anim = model->anims + model->anim_selected;
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

void ncx_model_destroy(NCXModel *model) {
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

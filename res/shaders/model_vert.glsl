#version 330 core

#define POINT_LIGHT_MAX 32
#define MAX_BONES 50

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;
layout(location = 3) in ivec3 a_bone_ids;
layout(location = 4) in vec3 a_weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool is_animated;
uniform mat4 bone_matrices[MAX_BONES];

uniform float time;
uniform float trip_intensity;
uniform int render_layer;

uniform vec3  light_points_pos;
uniform vec3  light_points_ambient_color;
uniform vec3  light_points_diffuse_color;
uniform vec3  light_points_specular_color;
uniform float light_points_constant;
uniform float light_points_linear;
uniform float light_points_quadratic;
uniform int   light_points_count_current;

out DATA {
	vec3 o_normal;
	vec2 o_uv;
	vec3 o_frag_pos;
	float o_time;
	float o_trip_intensity;
	mat4 o_projection;
	mat4 o_model;
	
	vec3  o_light_points_pos;
	vec3  o_light_points_ambient_color;
	vec3  o_light_points_diffuse_color;
	vec3  o_light_points_specular_color;
	float o_light_points_constant;
	float o_light_points_linear;
	float o_light_points_quadratic;
	int   o_light_points_count_current;
} data_out;

float rand(float n) {
	return fract(sin(n) * 43758.5453123);
}

float perlin_noise(float p) {
	float fl = floor(p);
	float fc = fract(p);
	return mix(rand(fl), rand(fl + 1.0), fc);
}

void main() {
	vec4 total_position = vec4(0.0);
	vec4 total_normal = vec4(0.0);
	if(is_animated) {
		for(int i = 0; i < 3; i++) {
			vec4 local_position = bone_matrices[a_bone_ids[i]] * vec4(a_pos, 1.0);
			vec4 world_normal = bone_matrices[a_bone_ids[i]] * vec4(a_normal, 0.0);
			total_position += local_position * a_weights[i];
			total_normal += world_normal * a_weights[i];
		}
	} else {
		total_position = vec4(a_pos, 1.0);
		total_normal = vec4(mat3(transpose(inverse(model))) * a_normal, 0.0);
	}

	gl_Position = view * model * total_position;

	vec2 trip_offset;
	trip_offset.x = perlin_noise((time * 1.57) + gl_Position.y) * (trip_intensity / 5);
	trip_offset.y = cos(time * 0.785 + gl_Position.z) * (trip_intensity / 5);
	gl_Position += vec4(trip_offset, 0.0, 0.0) * float(render_layer == 0);

	data_out.o_normal = total_normal.xyz;
	data_out.o_uv = a_uv;
	data_out.o_frag_pos = vec3(model * vec4(a_pos, 1.0));
	data_out.o_time = time;
	data_out.o_trip_intensity = trip_intensity;
	data_out.o_projection = projection;
	data_out.o_model = model;

	/* outputting light stuff */
	for(int i = 0; i < POINT_LIGHT_MAX; i++) {
		data_out.o_light_points_pos = light_points_pos;
		data_out.o_light_points_ambient_color = light_points_ambient_color;
		data_out.o_light_points_diffuse_color = light_points_diffuse_color;
		data_out.o_light_points_specular_color = light_points_specular_color;
		data_out.o_light_points_constant = light_points_constant;
		data_out.o_light_points_linear = light_points_linear;
		data_out.o_light_points_quadratic = light_points_quadratic;
	}
	data_out.o_light_points_count_current = light_points_count_current;
}

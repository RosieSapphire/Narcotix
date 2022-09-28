#version 330 core

#define MAX_BONES 50

layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;
layout(location = 3) in ivec3 bone_ids;
layout(location = 4) in vec3 weights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool is_animated;
uniform mat4 bone_matrices[MAX_BONES];

uniform float time;
uniform float trip_intensity;
uniform int render_layer;

/*
out DATA {
	vec3 normal;
	vec2 uv;
	vec3 frag_pos;
	float o_time;
	float o_trip_intensity;
	mat4 projection;
	mat4 model;
} data_out;
*/

out vec3 normal;
out vec2 uv;
out vec3 frag_pos;
out float o_time;
out float o_trip_intensity;

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
			vec4 local_position = bone_matrices[bone_ids[i]] * vec4(a_pos, 1.0);
			vec4 world_normal = bone_matrices[bone_ids[i]] * vec4(a_normal, 0.0);
			total_position += local_position * weights[i];
			total_normal += world_normal * weights[i];
		}
	} else {
		total_position = vec4(a_pos, 1.0);
		total_normal = vec4(mat3(transpose(inverse(model))) * a_normal, 0.0);
	}

	/*
	gl_Position = view * model * total_position;
	*/
	gl_Position = projection * view * model * total_position;

	vec2 trip_offset;
	trip_offset.x = perlin_noise((o_time * 1.57) + gl_Position.y) * (o_trip_intensity / 5);
	trip_offset.y = cos(o_time * 0.785 + gl_Position.z) * (o_trip_intensity / 5);
	gl_Position += vec4(trip_offset, 0.0, 0.0) * float(render_layer == 0);

	/*
	data_out.normal = total_normal.xyz;
	data_out.uv = a_uv;
	data_out.frag_pos = vec3(model * vec4(a_pos, 1.0));
	data_out.time = time;
	data_out.trip_intensity = trip_intensity;
	data_out.projection = projection;
	data_out.model = model;
	*/

	normal = total_normal.xyz;
	uv = a_uv;
	frag_pos = vec3(model * vec4(a_pos, 1.0));
	o_time = time;
	o_trip_intensity = trip_intensity;
}

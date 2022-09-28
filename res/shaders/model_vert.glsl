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

out vec3 o_normal;
out vec2 o_uv;
out vec3 o_frag_pos;
out float o_time;
out float o_trip_intensity;
out mat4 o_projection;
out mat4 o_model;

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

	o_normal = total_normal.xyz;
	o_uv = a_uv;
	o_frag_pos = vec3(model * vec4(a_pos, 1.0));
	o_time = time;
	o_trip_intensity = trip_intensity;
	o_projection = projection;
	o_model = model;
}

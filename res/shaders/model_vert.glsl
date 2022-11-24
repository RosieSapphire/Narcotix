#version 330 core

#define POINT_LIGHT_MAX 32
#define MAX_BONES 50

layout(location = 0) in vec3  a_pos;
layout(location = 1) in vec3  a_normal;
layout(location = 2) in vec2  a_uv;
layout(location = 3) in vec3  a_tangent;
layout(location = 4) in vec3  a_bitangent;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform float time;
uniform float trip_intensity;
uniform int   render_layer;
uniform vec3  view_pos;

out vec2  o_uv;
out vec3  o_frag_pos;
out vec3  o_view_pos;
out float o_time;
out float o_trip_intensity;
out mat4  o_projection;
out mat4  o_model;
out mat3  o_tbn;

float rand(float n) {
	return fract(sin(n) * 43758.5453123);
}

float perlin_noise(float p) {
	float fl = floor(p);
	float fc = fract(p);
	return mix(rand(fl), rand(fl + 1.0), fc);
}

void main() {
	// total_normal =   vec4(mat3(transpose(inverse(model))) * a_normal, 0.0);
	gl_Position = projection * view * model * vec4(a_pos, 1.0);

	vec2 trip_offset;
	trip_offset.x = perlin_noise((time * 1.57) + gl_Position.y) * (trip_intensity / 5);
	trip_offset.y = cos(time * 0.785 + gl_Position.z)           * (trip_intensity / 5);
	gl_Position += vec4(trip_offset, 0.0, 0.0)                  * float(render_layer == 0);

	vec3 t = normalize(vec3(model * vec4(a_tangent,   0.0)));
	vec3 b = normalize(vec3(model * vec4(a_bitangent, 0.0)));
	vec3 n = normalize(vec3(model * vec4(a_normal,    0.0)));
	o_tbn =            transpose(mat3(t, b, n));

	o_uv =             a_uv;
	o_frag_pos =       o_tbn * vec3(model * vec4(a_pos, 1.0));
	o_view_pos =       o_tbn * view_pos;
	o_time =           time;
	o_trip_intensity = trip_intensity;
	o_projection =     projection;
	o_model =          model;
}

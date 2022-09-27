#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec3 normal;
out vec2 uv;
out vec3 frag_pos;
out float time;
out float trip_intensity;

in DATA {
	vec3 normal;
	vec2 uv;
	vec3 frag_pos;
	float time;
	float trip_intensity;
	mat4 projection;
} data_in[];

void main() {
	for(int i = 0; i < 3; i++) {
		gl_Position = data_in[i].projection * gl_in[i].gl_Position;
		normal = data_in[i].normal;
		uv = data_in[i].uv;
		frag_pos = data_in[i].frag_pos;
		time = data_in[i].time;
		trip_intensity = data_in[i].trip_intensity;
		EmitVertex();
	}
	EndPrimitive();
}

#version 330 core

#define POINT_LIGHT_MAX 32

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 o_normal[];
in vec2 o_uv[];
in vec3 o_frag_pos[];
in float o_time[];
in float o_trip_intensity[];
in mat4 o_projection[];
in mat4 o_model[];

out vec3 f_normal;
out vec2 f_uv;
out vec3 f_frag_pos;
out float f_time;
out float f_trip_intensity;

void main() {
	/* calculate the TBN matrix for o_normal mapping */
	vec3 edge0 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 edge1 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec2 delta_uv0 = o_uv[1] - o_uv[0];
	vec2 delta_uv1 = o_uv[2] - o_uv[0];
	float inverse_delta = 1.0 / (delta_uv0.x * delta_uv1.y - delta_uv1.x * delta_uv0.y);

	vec3 tangent = vec3(inverse_delta * (delta_uv1.y * edge0 - delta_uv0.y * edge1));
	vec3 bitangent = vec3(inverse_delta * (-delta_uv1.x * edge0 - delta_uv0.x * edge1));

	vec3 t = normalize(vec3(o_model[0] * vec4(tangent, 0.0)));
	vec3 b = normalize(vec3(o_model[0] * vec4(bitangent, 0.0)));
	vec3 n = normalize(vec3(o_model[0] * vec4(cross(edge1, edge0), 0.0)));
	mat3 tbn = transpose(mat3(t, b, n));

	for(int i = 0; i < 3; i++) {
		gl_Position = o_projection[i] * gl_in[i].gl_Position;
		f_normal = o_normal[i];
		f_uv = o_uv[i];
		f_frag_pos = o_frag_pos[i];
		f_time = o_time[i];
		f_trip_intensity = o_trip_intensity[i];
		EmitVertex();
	}
	EndPrimitive();
}

#version 330 core

#define POINT_LIGHT_MAX 32

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

struct light_point_t {
	vec3 pos;
	vec3 ambient_color;
	vec3 diffuse_color;
	vec3 specular_color;
	float constant;
	float linear;
	float quadratic;
};

out vec3 normal;
out vec2 uv;
out vec3 frag_pos;
out float time;
out float trip_intensity;
out vec3 view_pos;
out light_point_t light_points[POINT_LIGHT_MAX];
out int light_points_count_current;

in DATA {
	vec3 normal;
	vec2 uv;
	vec3 frag_pos;
	float time;
	float trip_intensity;
	mat4 projection;
	mat4 model;
	vec3 view_pos;
	light_point_t light_points[POINT_LIGHT_MAX];
	int light_points_count_current;
} data_in[];

void main() {
	/* calculate TBN matrix for normal calculations */
	vec3 edge0 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 edge1 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec2 delta_uv0 = data_in[1].uv - data_in[0].uv;
	vec2 delta_uv1 = data_in[2].uv - data_in[0].uv;

	float inv_det = 1.0 / (delta_uv0.x * delta_uv1.y - delta_uv1.x * delta_uv0.y);

	vec3 tangent = vec3(inv_det * (delta_uv1.y * edge0 - delta_uv0.y * edge1));
	vec3 bitangent = vec3(inv_det * (-delta_uv1.x * edge0 + delta_uv0.x * edge1));

	vec3 t = normalize(vec3(data_in[0].model * vec4(tangent, 0.0)));
	vec3 b = normalize(vec3(data_in[0].model * vec4(bitangent, 0.0)));
	vec3 n = normalize(vec3(data_in[0].model * vec4(cross(edge1, edge0), 0.0)));
	mat3 tbn = transpose(mat3(t, b, n));

	/* add all the shit to all the vertices */
	for(int i = 0; i < 3; i++) {
		gl_Position =    data_in[i].projection * gl_in[i].gl_Position;
		normal =         data_in[i].normal;
		uv =             data_in[i].uv;
		frag_pos =       tbn * data_in[i].frag_pos;
		time =           data_in[i].time;
		trip_intensity = data_in[i].trip_intensity;
		view_pos =       tbn * data_in[i].view_pos;

		for(int j = 0; j < POINT_LIGHT_MAX; j++) {
			light_points[j] = data_in[i].light_points[j];
			light_points[j].pos = tbn * data_in[i].light_points[j].pos;
		}

		light_points_count_current = data_in[i].light_points_count_current;

		EmitVertex();
	}
	EndPrimitive();
}

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

in vec3 o_normal[];
in vec2 uv[];
in vec3 frag_pos[];
in float o_time[];
in float o_trip_intensity[];
in mat4 o_projection[];
in mat4 o_model[];
// in int o_light_points_count_current[];
/*
in light_point_t o_light_points[POINT_LIGHT_MAX];
*/

out vec3 f_normal;
out vec2 f_uv;
out vec3 f_frag_pos;
out float f_time;
out float f_trip_intensity;
// out int f_light_points_count_current;
/*
out light_point_t light_points[POINT_LIGHT_MAX];
*/

void main() {
	/* calculate the TBN matrix for o_normal mapping */
	vec3 edge0 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 edge1 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec2 delta_uv0 = uv[1] - uv[0];
	vec2 delta_uv1 = uv[2] - uv[0];
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
		f_uv = uv[i];
		f_frag_pos = frag_pos[i];
		f_time = o_time[i];
		f_trip_intensity = o_trip_intensity[i];
		// f_light_points_count_current = o_light_points_count_current[i];
		/*
		light_points = data_in[i].light_points;
		*/
		EmitVertex();
	}
	EndPrimitive();
}

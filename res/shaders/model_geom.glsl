#version 330 core

#define POINT_LIGHT_MAX 32

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in DATA {
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
} data_in[];

out vec3 f_normal;
out vec2 f_uv;
out vec3 f_frag_pos;
out float f_time;
out float f_trip_intensity;

out vec3  f_light_points_pos;
out vec3  f_light_points_ambient_color;
out vec3  f_light_points_diffuse_color;
out vec3  f_light_points_specular_color;
out float f_light_points_constant;
out float f_light_points_linear;
out float f_light_points_quadratic;
out int   f_light_points_count_current;

void main() {
	/* calculate the TBN matrix for o_normal mapping */
	vec3 edge0 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 edge1 = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec2 delta_uv0 = data_in[1].o_uv - data_in[0].o_uv;
	vec2 delta_uv1 = data_in[2].o_uv - data_in[0].o_uv;
	float inverse_delta = 1.0 / (delta_uv0.x * delta_uv1.y - delta_uv1.x * delta_uv0.y);

	vec3 tangent = vec3(inverse_delta * (delta_uv1.y * edge0 - delta_uv0.y * edge1));
	vec3 bitangent = vec3(inverse_delta * (-delta_uv1.x * edge0 - delta_uv0.x * edge1));

	vec3 t = normalize(vec3(data_in[0].o_model * vec4(tangent, 0.0)));
	vec3 b = normalize(vec3(data_in[0].o_model * vec4(bitangent, 0.0)));
	vec3 n = normalize(vec3(data_in[0].o_model * vec4(cross(edge1, edge0), 0.0)));
	mat3 tbn = transpose(mat3(t, b, n));

	for(int i = 0; i < 3; i++) {
		gl_Position =      data_in[i].o_projection * gl_in[i].gl_Position;
		f_normal =         data_in[i].o_normal;
		f_uv =             data_in[i].o_uv;
		f_frag_pos =       data_in[i].o_frag_pos;
		f_time =           data_in[i].o_time;
		f_trip_intensity = data_in[i].o_trip_intensity;

		for(int j = 0; j < 1; j++) {
			f_light_points_pos =            data_in[i].o_light_points_pos;
			f_light_points_ambient_color =  data_in[i].o_light_points_ambient_color;
			f_light_points_diffuse_color =  data_in[i].o_light_points_diffuse_color;
			f_light_points_specular_color = data_in[i].o_light_points_specular_color;
			f_light_points_constant =       data_in[i].o_light_points_constant;
			f_light_points_linear =         data_in[i].o_light_points_linear;
			f_light_points_quadratic =      data_in[i].o_light_points_quadratic;
		}

		EmitVertex();
	}
	EndPrimitive();
}

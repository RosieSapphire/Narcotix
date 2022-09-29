#version 330 core

#define POINT_LIGHT_MAX 32
#define NEAR 0.1
#define FAR 1000.0

in vec3 o_normal;
in vec2 o_uv;
in vec3 o_frag_pos;
in float o_time;
in float o_trip_intensity;
in mat4 o_projection;
in mat4 o_model;
in vec3  o_view_pos;
in mat3 o_tbn;

uniform vec3  light_points_pos;
uniform vec3  light_points_ambient_color;
uniform vec3  light_points_diffuse_color;
uniform vec3  light_points_specular_color;
uniform float light_points_constant;
uniform float light_points_linear;
uniform float light_points_quadratic;
uniform int   light_points_count_current;

out vec4 frag_color;

struct material_t {
	sampler2D tex_diffuse;
	sampler2D tex_specular;
	sampler2D tex_normal;
	float shininess;
}; uniform material_t material;

float rand(float n) {
	return fract(sin(n) * 43758.5453123);
}

float perlin_noise(float p){
	float fl = floor(p);
	float fc = fract(p);
	return mix(rand(fl), rand(fl + 1.0), fc);
}

vec3 get_light_point(vec3 pos, vec3 ambient_color, vec3 diffuse_color, vec3 specular_color, float constant, float linear, float quadratic, vec3 normal) {
	vec3 light_dir = o_tbn * normalize(pos - o_frag_pos);
	vec3 view_dir = o_tbn * normalize(o_view_pos - o_frag_pos);
	float diffuse_amount = max(dot(normal, light_dir), 0.0);
	vec3 reflect_dir = reflect(-light_dir, normal);
	float specular_amount = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
	float distance = length(pos - o_frag_pos);
	float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

	vec2 trip_uv = vec2(o_uv.x + sin(o_time * 0.1f + o_trip_intensity) * o_trip_intensity * 0.12f, o_uv.y + cos(o_time * 0.2f + o_trip_intensity) * o_trip_intensity * 0.04f);
	vec3 final_ambient_color = ambient_color * vec3(texture(material.tex_diffuse, trip_uv));
	vec3 final_diffuse_color = diffuse_color * diffuse_amount * vec3(texture(material.tex_diffuse, trip_uv));
	vec3 final_specular_color = specular_color * specular_amount * vec3(texture(material.tex_specular, trip_uv).r);
	final_ambient_color *= attenuation;
	final_diffuse_color *= attenuation;
	final_specular_color *= attenuation;
	return (final_ambient_color + final_diffuse_color + final_specular_color);
}

/* TODO: Add directional lights at some point */

void main() {
	// vec3 norm = normalize(o_tbn * normalize(texture(material.tex_normal, o_uv).xyz / 2.0 - 1.0));
	// vec3 norm = texture(material.tex_normal, o_uv).rgb;
	vec3 norm = normalize(texture(material.tex_normal, o_uv).xyz * 2.0 - 1.0);
	vec3 final_color = vec3(0.0);

	/*
	for(int i = 0; i < POINT_LIGHT_MAX; i++)
		final_color += get_light_point(light_points_pos[i], light_points_ambient_color[i], light_points_specular_color[i], light_points_diffuse_color[i], light_points_constant[i], light_points_linear[i], light_points_quadratic[i], norm);
		*/

	final_color += get_light_point(light_points_pos, light_points_ambient_color, light_points_specular_color, light_points_diffuse_color, light_points_constant, light_points_linear, light_points_quadratic, norm);

	vec3 trippy_color;
	trippy_color.x = (cos(o_time * 2) + 1.0f) / 2;
	trippy_color.y = 1.0 - ((sin(o_time * 4) + 1.0f) / 2);
	trippy_color.z = ((sin(o_time * 4) * cos(o_time * 2)) + 1.0f) / 3;
	trippy_color *= vec3(perlin_noise(o_view_pos.x), perlin_noise(o_view_pos.y), perlin_noise(o_view_pos.z / 2)) + o_frag_pos + sin(o_time);
	trippy_color *= (o_trip_intensity / 4.0);

	frag_color = vec4(final_color, 1.0) + vec4(trippy_color / 2, 1.0);
}

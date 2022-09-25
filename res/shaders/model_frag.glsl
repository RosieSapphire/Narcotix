#version 330

#define POINT_LIGHT_MAX 32

in vec3 normal;
in vec2 uv;
in vec3 frag_pos;

in float o_time;
in float o_trip_intensity;

out vec4 FragColor;

uniform vec3 view_pos;

struct material_t {
	sampler2D tex_diffuse;
	sampler2D tex_specular;
	float shininess;
}; uniform material_t material;

struct light_point_t {
	vec3 pos;
	vec3 ambient_color;
	vec3 diffuse_color;
	vec3 specular_color;
	float constant;
	float linear;
	float quadratic;
}; uniform light_point_t light_points[POINT_LIGHT_MAX];
uniform int light_points_count_current;

float rand(float n) {
	return fract(sin(n) * 43758.5453123);
}

float perlin_noise(float p){
	float fl = floor(p);
	float fc = fract(p);
	return mix(rand(fl), rand(fl + 1.0), fc);
}

float near = 0.1;
float far = 1000.0;

vec3 get_light_point(light_point_t l, vec3 normal, vec3 frag_pos, vec3 view_dir) {
	vec3 light_dir = normalize(l.pos - frag_pos);
	float diffuse_amount = max(dot(normal, light_dir), 0.0);
	vec3 reflect_dir = reflect(-light_dir, normal);
	float specular_amount = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
	float distance = length(l.pos - frag_pos);
	float attenuation = 1.0 / (l.constant + l.linear * distance + l.quadratic * (distance * distance));

	vec2 trip_uv = vec2(uv.x + sin(o_time * 0.1f + o_trip_intensity) * o_trip_intensity * 0.12f, uv.y + cos(o_time * 0.2f + o_trip_intensity) * o_trip_intensity * 0.04f);
	vec3 ambient_color = l.ambient_color * vec3(texture(material.tex_diffuse, trip_uv));
	vec3 diffuse_color = l.diffuse_color * diffuse_amount * vec3(texture(material.tex_diffuse, trip_uv));
	vec3 specular_color = l.specular_color * specular_amount * vec3(texture(material.tex_specular, trip_uv).r);
	ambient_color *= attenuation;
	diffuse_color *= attenuation;
	specular_color *= attenuation;
	return (ambient_color + diffuse_color + specular_color);
}

/* TODO: Add directional lights at some point */
/*
struct light_dir_t {
	vec3 dir;
	vec3 ambient_color;
	vec3 diffuse_color;
	vec3 specular_color;
}; uniform light_dir_t light_dir;
*/

/*
vec3 get_light_dir(light_dir_t l, vec3 normal, vec3 view_dir) {
	vec3 light_dir = normalize(-l.dir);
	float diffuse_amount = max(dot(normal, light_dir), 0.0);
	vec3 reflect_dir = reflect(-light_dir, normal);
	float specular_amount = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);

	vec2 trip_uv_offset = vec2(uv.x + sin(o_time * 0.2f + o_trip_intensity) * o_trip_intensity * 0.43f, uv.y + cos(o_time * 0.4f + o_trip_intensity) * o_trip_intensity * 0.12f);
	vec3 ambient_color = l.ambient_color * vec3(texture(material.tex_diffuse, trip_uv_offset));
	vec3 diffuse_color = l.diffuse_color * diffuse_amount * vec3(texture(material.tex_diffuse, trip_uv_offset));
	vec3 specular_color = l.specular_color * specular_amount * vec3(texture(material.tex_specular, trip_uv_offset).r);
	return (ambient_color + diffuse_color + specular_color);
}
*/

void main() {
	vec3 norm = normalize(normal);
	vec3 view_dir = normalize(view_pos - frag_pos);
	// vec3 final_color = get_light_dir(light_dir, norm, view_dir);
	vec3 final_color = vec3(0.0f);

	for(int i = 0; i < light_points_count_current; i++)
		final_color += get_light_point(light_points[i], norm, frag_pos, view_dir);

	vec3 trippy_color;
	trippy_color.x = (cos(o_time * 2) + 1.0f) / 2;
	trippy_color.y = 1.0 - ((sin(o_time * 4) + 1.0f) / 2);
	trippy_color.z = ((sin(o_time * 4) * cos(o_time * 2)) + 1.0f) / 3;
	trippy_color *= vec3(perlin_noise(view_pos.x), perlin_noise(view_pos.y), perlin_noise(view_pos.z / 2)) + frag_pos + sin(o_time);
	trippy_color *= (o_trip_intensity / 4.0);

	FragColor = vec4(final_color, 1.0) + vec4(trippy_color / 2, 1.0);
}

#version 330 core

in vec2 uv;
in vec4 frag_pos;

uniform sampler2D screen_texture;
uniform sampler2D trippy_texture;
uniform float time;
uniform float trip_intensity;

uniform bool use_trippy_effect;

out vec4 frag_color;

void main() {
	vec4 base_color;
	vec4 trippy_color;

	/* shamelessly lifted from https://www.shadertoy.com/view/XsVSDz */
	vec2 vp = vec2(320.0, 200.0);
	float time10 = time * 10.0;
    vec2 p0 = (uv - 0.5) * vp;
    vec2 hvp = vp * 0.5;
	vec2 p1d = vec2(cos( time10 / 98.0),  sin( time10 / 178.0)) * hvp - p0;
	vec2 p2d = vec2(sin(-time10 / 124.0), cos(-time10 / 104.0)) * hvp - p0;
	vec2 p3d = vec2(cos(-time10 / 165.0), cos( time10 / 45.0))  * hvp - p0;
    float sum = 0.5 + 0.5 * (cos(length(p1d) / 30.0) + cos(length(p2d) / 20.0) + sin(length(p3d) / 25.0) * sin(p3d.x / 20.0) * sin(p3d.y / 15.0));
	trippy_color = texture(trippy_texture, vec2(fract(sum), 0));

	vec2 warbly_uv = vec2(uv.x + ((sin(uv.y * 4 + time) / 64) * trip_intensity), uv.y + ((cos(uv.x * 4 + time) / 64) * trip_intensity));
	warbly_uv.x += trippy_color.r * 0.01f * trip_intensity;
	warbly_uv.y += trippy_color.b * 0.01f * trip_intensity;
    base_color = texture(screen_texture, warbly_uv);

	frag_color = base_color;
	if(use_trippy_effect) {
		frag_color = 1 - (1 - base_color) * (1 - (trippy_color * trip_intensity * 0.2));
	}
}

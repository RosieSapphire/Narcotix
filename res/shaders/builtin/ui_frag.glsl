#version 330 core

in vec2 uv;

uniform sampler2D tex;
uniform float flash_amount;

out vec4 frag_color;

void main() {
	frag_color = texture(tex, uv) + vec4(flash_amount, 0.0, 0.0, 0.0);
}

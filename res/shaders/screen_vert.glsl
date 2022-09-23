#version 330 core
layout(location = 0) in vec4 a_vertex;

out vec2 uv;
out vec4 frag_pos;

void main() {
	gl_Position = vec4(a_vertex.xy, 0.0, 1.0);
	uv = a_vertex.zw;
	frag_pos = gl_Position;
}

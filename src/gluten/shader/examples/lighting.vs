#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

out vec4 VertexColor;
out vec2 UvMap;

uniform mat4 transform;

void main() {
    gl_Position = transform * vec4(pos, 1.0f);
	// VertexColor = color;
	UvMap = uv;
}
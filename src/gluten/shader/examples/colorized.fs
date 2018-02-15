#version 330 core
out vec4 FragmentColor;
in vec4 VertexColor;

uniform vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

void main() {
    FragmentColor = color * VertexColor;
} 
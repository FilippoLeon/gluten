#version 330 core
in vec4 VertexColor;
in vec2 UvMap;

out vec4 FragmentColor;

uniform vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

uniform sampler2D texture0;

struct Ambient {
    float strength = 0.1;
    vec3 color = vec3(1.0f, 1.0f, 1.0f);
};

uniform Ambient ambient;

void main() {
	vec3 ambientColor = ambient.strength * ambient.color;
    FragmentColor = color * ambientColor * texture(texture0, UvMap);
} 
out vec4 FragmentColor;

in vec4 VertexColor;
in vec2 UvMap;

uniform vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

uniform sampler2D texture0;

void main() {
    FragmentColor = color * texture(texture0, UvMap);
} 
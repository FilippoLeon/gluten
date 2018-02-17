in vec2 UvMap;

out vec4 FragColor;

uniform sampler2D textBuffer;
uniform vec3 textColor;

void main() {    
    FragColor = vec4(textColor, texture(textBuffer, UvMap).r);
}  
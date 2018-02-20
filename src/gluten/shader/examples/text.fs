in vec2 UvMap;

out vec4 FragColor;

uniform sampler2D textBuffer;
uniform vec4 textColor;

void main() {    
    FragColor = vec4(vec3(textColor), texture(textBuffer, UvMap).r);
}  
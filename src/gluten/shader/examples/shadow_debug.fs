out vec4 FragmentColor;
  
in vec2 UvMap;

uniform sampler2D depthMap;

void main() {             
    FragmentColor = vec4(texture(depthMap, UvMap).r, 0, 0, 1.0);
}  
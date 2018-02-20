out vec4 FragmentColor;
  
in vec2 UvMap;

uniform sampler2D depthMap;

void main() {             
    FragmentColor = vec4(vec3(texture(depthMap, UvMap).r), 1.0);
}  
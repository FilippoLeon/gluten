layout (location = 0) in vec3 pos;

uniform mat4 transform;
uniform mat4 modelView;

void main() {
    gl_Position = transform * modelView * vec4(pos, 1.0);
}  
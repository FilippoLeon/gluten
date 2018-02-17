layout (location = 0) in vec4 pos;
layout (location = 0) in vec4 uv;

out vec2 UvMap;

uniform mat4 projection;

void main() {
	UvMap = uv;
    gl_Position = projection * vec4(pos, 0.0, 1.0);
}  
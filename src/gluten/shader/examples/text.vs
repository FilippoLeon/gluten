layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;

out vec2 UvMap;

uniform mat4 projection;

uniform vec2 size;
uniform vec2 position;

void main() {
	UvMap = uv;

	vec3 loc_pos = vec3(pos.x + 0.5, -pos.y + 0.5, 0) ;
	vec3 scale = vec3(size, 1.0);
    gl_Position =  projection * vec4(loc_pos*scale + vec3(position, 0.0), 1.0);
}  
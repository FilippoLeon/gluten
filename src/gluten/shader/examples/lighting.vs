layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

out vec4 VertexColor;
out vec2 UvMap;
out vec3 WorldPosition;
out vec3 Normal;

uniform mat4 transform;
uniform mat4 modelView;
uniform mat3 jacobianTransform;

void main() {
	vec4 tempPos = modelView * vec4(pos, 1.0);
	WorldPosition = vec3(tempPos);
    gl_Position = transform * tempPos;
	// VertexColor = color;
	UvMap = uv;
	Normal = jacobianTransform * normal;
}
in vec4 VertexColor;
in vec2 UvMap;
in vec3 WorldPosition;
in vec3 Normal;

out vec4 FragmentColor;

uniform vec4 tint = vec4(1.0f, 1.0f, 1.0f, 1.0f);

uniform vec3 viewPosition;

struct Light {
	vec3 position;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};

uniform Light light;

struct Material {
	#ifdef TEXTURE
    sampler2D diffuse;
    sampler2D specular;
	#else
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
	#endif

	float shininess;
};

uniform Material material;

void main() {
	#ifdef TEXTURE
	vec3 matDiffuseColor = vec3(texture(material.diffuse, UvMap));
	vec3 matAmbientColor = matDiffuseColor;
	vec3 matSpecularColor = vec3(texture(material.specular, UvMap));
	#else
	vec3 matDiffuseColor = material.diffuseColor;
	vec3 matAmbientColor = material.ambientColor;
	vec3 matSpecularColor = material.specularColor;
	#endif
	
	vec3 unitNormal = normalize(Normal);
	vec3 unitLightDirection = normalize(light.position - WorldPosition);  
	float diffuseIntensity = max(dot(unitNormal, unitLightDirection), 0.0);
	vec3 diffuseColor = diffuseIntensity * light.diffuseColor * matDiffuseColor;

	vec3 viewDirection = normalize(viewPosition - WorldPosition);
	vec3 reflectDirection = reflect(-unitLightDirection, unitNormal);  
	float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
	vec3 specularColor = specularIntensity * light.specularColor * matSpecularColor; 
	
	vec3 ambientColor = light.ambientColor * matAmbientColor;

    FragmentColor = tint * vec4(ambientColor + diffuseColor + specularColor, 1.0f);
} 
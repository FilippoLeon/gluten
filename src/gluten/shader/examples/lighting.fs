in vec4 VertexColor;
in vec2 UvMap;
in vec3 WorldPosition;
in vec3 Normal;

out vec4 FragmentColor;

uniform vec4 tint = vec4(1.0f, 1.0f, 1.0f, 1.0f);

uniform vec3 viewPosition;

struct Light {
	vec3 position;
	vec3 direction;

	float radius;
	vec2 cutoff;

	int type;

	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;

	vec3 fade;
};

#define N_LIGHTS 10
uniform Light lights[N_LIGHTS];

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

vec3 lighting(Light light, vec3 unitNormal, 
              vec3 matAmbientColor, 
              vec3 matDiffuseColor, 
			  vec3 matSpecularColor) {
	float attenuation = 1.0f;

	vec3 unitLightDirection;
	float factor = 1.0f;
	if ( light.type == 0 ){
		unitLightDirection = normalize(light.position - WorldPosition);  
		float lightDistance = length(light.position - WorldPosition);
		attenuation = 1.0 / (light.fade.x + light.fade.y * lightDistance + 
    				  light.fade.z * (lightDistance * lightDistance)); 
	} else if ( light.type == 1 ) {
	    unitLightDirection = normalize(light.direction);
	} else if ( light.type == 2 ) {
		unitLightDirection = normalize(light.position - WorldPosition);  
		float r = dot(unitLightDirection, normalize(-light.direction));
		float eps = light.cutoff.x - light.cutoff.y;
		factor = clamp((r - light.cutoff.y) / eps, 0.0, 1.0); 
	}

	vec3 ambientColor = light.ambientColor * matAmbientColor;

	float diffuseIntensity = max(dot(unitNormal, unitLightDirection), 0.0);
	diffuseIntensity  *= attenuation;
	vec3 diffuseColor = diffuseIntensity * light.diffuseColor * matDiffuseColor;

	vec3 viewDirection = normalize(viewPosition - WorldPosition);
	vec3 reflectDirection = reflect(-unitLightDirection, unitNormal);  
	float specularIntensity = pow(max(dot(viewDirection, reflectDirection), 0.0), material.shininess);
	specularIntensity *= attenuation; 
	vec3 specularColor = specularIntensity * light.specularColor * matSpecularColor; 

	return ambientColor + factor * (diffuseColor + specularColor);
}

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

	vec3 result = vec3(0, 0, 0);
    for(int i = 0; i < N_LIGHTS; i++) {
		if(lights[i].type == -1) {
			break;
		}
        result += lighting(lights[i], unitNormal, 
		                   matAmbientColor, matDiffuseColor, matSpecularColor);
	}

    FragmentColor = tint * vec4(result, 1.0f);
} 
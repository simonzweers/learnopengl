#version 330 core

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

uniform Material material;
uniform Light light;

out vec4 FragColor;
  
uniform vec3 objectColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

void main()
{
	vec3 lightDir = normalize(light.position - FragPos);

	// Ambient
	vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

	// Diffuse
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// Specular
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	// Spotlight
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); 
	diffuse *= intensity;
	specular *= intensity;

	// Attenuation
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (
			light.constant +
			light.linear * distance +
			light.quadratic * (distance * distance)
			);
	ambient  *= attenuation; 
	diffuse  *= attenuation;
	specular *= attenuation;   

	vec3 result = (ambient + diffuse + specular);
	FragColor = vec4(result, 1.0);
}

#version 330 core

in vec3 vertOutFragPos;
in vec3 vertOutNormal;

uniform	vec3 u_ObjectColor;
uniform	vec3 u_LightColor;
uniform	vec3 u_ViewPos;
uniform	vec3 u_LightPos;

out vec4 color;

void main()
{
	// ambient light (always small amount of light)
	float flAmbientFactor = 0.1;
	vec3 ambient = flAmbientFactor * u_LightColor;
	
	// diffuse
	vec3 norm = normalize(vertOutNormal);
	vec3 lightDir = normalize(u_LightPos - vertOutFragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * u_LightColor;
	
	// specular
	float specularFactor = 0.5;
	vec3 viewDir = normalize(u_ViewPos - vertOutFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularFactor * spec * u_LightColor;
	
	vec3 result = (ambient + diffuse + specular) * u_ObjectColor;
	color = vec4(result, 1);
}
#version 420 core

in vec3 vertOutFragPos;
in vec3 vertOutNormal;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 
  
uniform Material u_Material;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light u_Light;  

uniform	vec3 u_ObjectColor;
uniform	vec3 u_ViewPos;
uniform	vec3 u_LightPos;

out vec4 color;

void main()
{
	// ambient light (always small amount of light)
	
	// diffuse
	vec3 norm = normalize(vertOutNormal);
	vec3 lightDir = normalize(u_LightPos - vertOutFragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	
	// specular
	vec3 viewDir = normalize(u_ViewPos - vertOutFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
	
	vec3 ambient  = u_Light.ambient * u_Material.ambient;
	vec3 diffuse  = u_Light.diffuse * (diff * u_Material.diffuse);
	vec3 specular = u_Light.specular * (spec * u_Material.specular);  
	
	vec3 result = (ambient + diffuse + specular) * u_ObjectColor;
	color = vec4(result, 1);
}
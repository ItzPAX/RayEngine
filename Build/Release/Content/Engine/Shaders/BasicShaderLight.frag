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

struct DirectionalLight
{
	vec3 m_Direction;
	vec3 m_Ambient;
	vec3 m_Diffuse;
	vec3 m_Specular;
};
uniform DirectionalLight u_DirectionalLight;  

struct SpotLight
{
	vec3 m_Position;
	vec3 m_Direction;
	vec3 m_Ambient;
	vec3 m_Diffuse;
	vec3 m_Specular;
	
	float m_OuterCutOff;
	float m_InnerCutOff;
	
	float m_Constant;
	float m_Linear;
	float m_Quadratic;
};
uniform SpotLight u_SpotLight;  

struct PointLight
{
	vec3 m_Position;
	vec3 m_Ambient;
	vec3 m_Diffuse;
	vec3 m_Specular;
	
	float m_Constant;
	float m_Linear;
	float m_Quadratic;
};
uniform PointLight u_PointLights[256];

uniform	vec3 u_ObjectColor;
uniform	vec3 u_ViewPos;
uniform int u_PointLightAmount;

out vec4 color;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.m_Direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // combine results
    vec3 ambient  = light.m_Ambient * u_Material.ambient;
    vec3 diffuse  = light.m_Diffuse * (diff * u_Material.diffuse);
    vec3 specular = light.m_Specular * (spec * u_Material.specular);  
    return (ambient + diffuse + specular);
} 

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.m_Position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // attenuation
    float distance    = length(light.m_Position - fragPos);
    float attenuation = 1.0 / (light.m_Constant + light.m_Linear * distance + light.m_Quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.m_Ambient * u_Material.ambient;
    vec3 diffuse  = light.m_Diffuse * (diff * u_Material.diffuse);
    vec3 specular = light.m_Specular * (spec * u_Material.specular);  
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
} 

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.m_Position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // attenuation
    float distance    = length(light.m_Position - fragPos);
    float attenuation = 1.0 / (light.m_Constant + light.m_Linear * distance + light.m_Quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = light.m_Ambient * u_Material.ambient;
    vec3 diffuse  = light.m_Diffuse * (diff * u_Material.diffuse);
    vec3 specular = light.m_Specular * (spec * u_Material.specular);  
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
	
	float theta = dot(lightDir, normalize(-light.m_Direction));
	float epsilon = light.m_InnerCutOff - light.m_OuterCutOff;
	float intensity = clamp((theta - light.m_OuterCutOff) / epsilon, 0.0, 1.0);  
		
	diffuse *= intensity;
	specular *= intensity;
		
    return (ambient + diffuse + specular);
}

void main()
{
    // properties
    vec3 norm = normalize(vertOutNormal);
    vec3 viewDir = normalize(u_ViewPos - vertOutFragPos);

    // phase 1: Directional lighting
    vec3 result = CalcDirLight(u_DirectionalLight, norm, viewDir);
    // phase 2: Point lights
    for(int i = 0; i < u_PointLightAmount; i++)
        result += CalcPointLight(u_PointLights[i], norm, vertOutFragPos, viewDir);    
    // phase 3: Spot light
    result += CalcSpotLight(u_SpotLight, norm, vertOutFragPos, viewDir);    
    
    color = vec4(result, 1.0);
}
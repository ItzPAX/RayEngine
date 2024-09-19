#version 460 core

in mat3 vertOutTBN;
in vec3 vertOutFragPos;
in vec2 vertOutTexCoords;

out vec4 color;

struct Material {
    float shininess;
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_normal1;
	sampler2D texture_ambient1;
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
	
	int m_Index; // not important
};
uniform PointLight u_PointLights[64];

//layout(std430, binding = 1) buffer PointLightBuffer
//{
//    PointLight u_PointLights[];
//};

uniform	vec3 u_ObjectColor;
uniform	vec3 u_ViewPos;
uniform int u_PointLightAmount;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.m_Direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
    // combine results
    vec3 ambient  = light.m_Ambient * vec3(texture(u_Material.texture_ambient1, vertOutTexCoords));// * vec3(texture(u_Material.texture_ambient1, vertOutTexCoords));
    vec3 diffuse  = light.m_Diffuse * diff * vec3(texture(u_Material.texture_diffuse1, vertOutTexCoords));
    vec3 specular = light.m_Specular * spec * vec3(texture(u_Material.texture_specular1, vertOutTexCoords));
	
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
    vec3 ambient  = light.m_Ambient * vec3(texture(u_Material.texture_ambient1, vertOutTexCoords));// * vec3(texture(u_Material.texture_ambient1, vertOutTexCoords));
    vec3 diffuse  = light.m_Diffuse * diff * vec3(texture(u_Material.texture_diffuse1, vertOutTexCoords));
    vec3 specular = light.m_Specular * spec * vec3(texture(u_Material.texture_specular1, vertOutTexCoords));
	
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
    vec3 ambient  = light.m_Ambient * vec3(texture(u_Material.texture_ambient1, vertOutTexCoords));// * vec3(texture(u_Material.texture_ambient1, vertOutTexCoords));
    vec3 diffuse  = light.m_Diffuse * diff * vec3(texture(u_Material.texture_diffuse1, vertOutTexCoords));
    vec3 specular = light.m_Specular * spec * vec3(texture(u_Material.texture_specular1, vertOutTexCoords));
	
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
	
	float theta = dot(lightDir, normalize(-light.m_Direction));
	float epsilon = light.m_InnerCutOff - light.m_OuterCutOff;
	float intensity = clamp((theta - light.m_OuterCutOff) / epsilon, 0.0, 1.0);  
	
	ambient *= intensity;
	diffuse *= intensity;
	specular *= intensity;
		
    return (ambient + diffuse + specular);
}

void main()
{
	// discard transparent pixels
	vec4 diffuse = texture(u_Material.texture_diffuse1, vertOutTexCoords);
	if(diffuse.a < 0.1)
        discard;
		
    // properties
	vec3 norm = texture(u_Material.texture_normal1, vertOutTexCoords).rgb;
	norm = norm * 2.0 - 1.0;   
	norm = normalize(vertOutTBN * norm); 

    vec3 viewDir = normalize(u_ViewPos - vertOutFragPos);
	
    // phase 1: Directional lighting
    vec3 result = CalcDirLight(u_DirectionalLight, norm, viewDir);
	
    // phase 2: Point lights
	for (int i = 0; i < u_PointLightAmount; ++i) {
		result += CalcPointLight(u_PointLights[i], norm, vertOutFragPos, viewDir);
	}
	
    // phase 3: Spot light
    result += CalcSpotLight(u_SpotLight, norm, vertOutFragPos, viewDir);    
	
	color = vec4(result, 1);
}
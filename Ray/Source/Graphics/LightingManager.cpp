#include "Ray.h"

void LightingManager::ManageBasicLighting(ShaderProgramPtr shader, const LightingDesc& light, glm::vec3 objectcolor, const MaterialDesc& material)
{
	// set shader uniforms
	shader->SetVec3("u_ObjectColor", objectcolor);
	shader->SetVec3("u_ViewPos", FloatingCamera::GetFloatingCam().Position());

	shader->SetVec3("u_Material.ambient", material.m_Ambient);
	shader->SetVec3("u_Material.diffuse", material.m_Diffuse);
	shader->SetVec3("u_Material.specular", material.m_Specular);
	shader->SetFloat("u_Material.shininess", 32.0f);

	shader->SetVec3("u_Light.ambient", light.m_Ambient);
	shader->SetVec3("u_Light.diffuse", light.m_Diffuse);
	shader->SetVec3("u_Light.specular", light.m_Specular);

	shader->SetVec3("u_LightPos", light.m_Position);
}
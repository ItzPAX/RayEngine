#include "Ray.h"

void LightingManager::ManageBasicLighting(ShaderProgramPtr shader, const LightingDesc& desc)
{
	// set shader uniforms
	shader->SetVec3("u_ObjectColor", desc.m_ObjectColor);
	shader->SetVec3("u_LightColor", desc.m_LightColor);
	shader->SetVec3("u_ViewPos", desc.m_CameraPos);
	shader->SetVec3("u_LightPos", glm::vec3(1, 1, 1));
}
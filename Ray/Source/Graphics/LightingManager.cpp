#include "Ray.h"

void LightingManager::ManageBasicLighting(ShaderProgramPtr shader, const LightingDesc& desc)
{
	// set shader uniforms
	shader->SetVec3("u_ObjectColor", desc.m_ObjectColor);
	shader->SetVec3("u_LightColor", desc.m_LightColor);
	shader->SetVec3("u_ViewPos", FloatingCamera::GetFloatingCam().Position());

	if (!PrimitiveContainer::Instance().m_LightPositions.empty())
		shader->SetVec3("u_LightPos", PrimitiveContainer::Instance().m_LightPositions.front());
}
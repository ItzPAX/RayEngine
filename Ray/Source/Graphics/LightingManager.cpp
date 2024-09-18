#include "Ray.h"

void LightingManager::ManageBasicLighting(ShaderProgramPtr shader, glm::vec3 objectcolor, const MaterialDesc& material)
{
	// object variables
	shader->SetVec3("u_ObjectColor", objectcolor);

	// camera
	shader->SetVec3("u_ViewPos", FloatingCamera::GetFloatingCam().Position());

	// object material
	shader->SetFloat("u_Material.shininess", 32.0f);

	// directional light
	shader->SetVec3("u_DirectionalLight.m_Direction", LightingManager::Instance().m_DirectionalLight.m_Direction);
	shader->SetVec3("u_DirectionalLight.m_Ambient", LightingManager::Instance().m_DirectionalLight.m_Ambient);
	shader->SetVec3("u_DirectionalLight.m_Diffuse", LightingManager::Instance().m_DirectionalLight.m_Diffuse);
	shader->SetVec3("u_DirectionalLight.m_Specular", LightingManager::Instance().m_DirectionalLight.m_Specular);

	// spotlight light
	shader->SetVec3("u_SpotLight.m_Position", LightingManager::Instance().m_SpotLight.m_Position);
	shader->SetVec3("u_SpotLight.m_Direction", LightingManager::Instance().m_SpotLight.m_Direction);
	shader->SetVec3("u_SpotLight.m_Ambient", LightingManager::Instance().m_SpotLight.m_Ambient);
	shader->SetVec3("u_SpotLight.m_Diffuse", LightingManager::Instance().m_SpotLight.m_Diffuse);
	shader->SetVec3("u_SpotLight.m_Specular", LightingManager::Instance().m_SpotLight.m_Specular);

	shader->SetFloat("u_SpotLight.m_OuterCutOff", LightingManager::Instance().m_SpotLight.m_OuterCutOff);
	shader->SetFloat("u_SpotLight.m_InnerCutOff", LightingManager::Instance().m_SpotLight.m_InnerCutOff);

	shader->SetFloat("u_SpotLight.m_Constant", LightingManager::Instance().m_SpotLight.m_Constant);
	shader->SetFloat("u_SpotLight.m_Linear", LightingManager::Instance().m_SpotLight.m_Linear);
	shader->SetFloat("u_SpotLight.m_Quadratic", LightingManager::Instance().m_SpotLight.m_Quadratic);

	// point lights
	shader->SetInt("u_PointLightAmount", m_PointLights.size());

	for (auto& pointlight_map : m_PointLights)
	{
		PointLight pl = pointlight_map.second;
		std::string indexed_pointlight = shader->GetIndexedArrayUniformName(pointlight_map.first, "u_PointLights");
	
		shader->SetVec3((indexed_pointlight + ".m_Position").c_str(), pl.m_Position);
		shader->SetVec3((indexed_pointlight + ".m_Ambient").c_str(), pl.m_Ambient);
		shader->SetVec3((indexed_pointlight + ".m_Diffuse").c_str(), pl.m_Diffuse);
		shader->SetVec3((indexed_pointlight + ".m_Specular").c_str(), pl.m_Specular);
	
		shader->SetFloat((indexed_pointlight + ".m_Constant").c_str(), pl.m_Constant);
		shader->SetFloat((indexed_pointlight + ".m_Linear").c_str(), pl.m_Linear);
		shader->SetFloat((indexed_pointlight + ".m_Quadratic").c_str(), pl.m_Quadratic);
	}
}

void LightingManager::DeletePointLight(const PointLight& light)
{
	m_PointLights.erase(light.m_Index);

	m_ContiguousPointLights.clear();
	for (const auto& pair : m_PointLights) {
		m_ContiguousPointLights.push_back(pair.second);
	}
}

PointLightPtr LightingManager::AddPointLight(PointLight& light)
{
	light.m_Index = m_PointLights.size();
	m_PointLights[light.m_Index] = light;

	m_ContiguousPointLights.clear();
	for (const auto& pair : m_PointLights) {
		m_ContiguousPointLights.push_back(pair.second);
	}

	return std::make_shared<PointLight>(light);
}
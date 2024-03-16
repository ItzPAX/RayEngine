#pragma once
#include <unordered_map>

// if you change this change it in the fragment shader aswell
#define MAX_POINT_LIGHTS 256

struct RAY_API DirectionalLight
{
	glm::vec3 m_Direction;
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
};

struct RAY_API SpotLight
{
	glm::vec3 m_Position;
	glm::vec3 m_Direction;
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;

	float m_OuterCutOff;
	float m_InnerCutOff;

	float m_Constant;
	float m_Linear;
	float m_Quadratic;
};

struct RAY_API PointLight
{
	glm::vec3 m_Position;
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;

	float m_Constant;
	float m_Linear;
	float m_Quadratic;

	int m_Index;
};

struct RAY_API MaterialDesc
{
	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
	float m_Shininess;
};

class RAY_API LightingManager
{
public:
	static LightingManager& Instance()
	{
		static LightingManager instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}
private:
	LightingManager() {}                    // Constructor? (the {} brackets) are needed here.

public:
	LightingManager(LightingManager const&) = delete;
	void operator=(LightingManager const&) = delete;

public:
	void ManageBasicLighting(ShaderProgramPtr shader, glm::vec3 objectcolor, const MaterialDesc& material);
	void AddPointLight(PointLight& light);
	void DeletePointLight(const PointLight& light);

	DirectionalLight m_DirectionalLight;
	SpotLight m_SpotLight;
	std::unordered_map<int, PointLight> m_PointLights;

private:
};
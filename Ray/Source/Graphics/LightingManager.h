#pragma once

struct RAY_API LightingDesc
{
	glm::vec3 m_Position;

	glm::vec3 m_Ambient;
	glm::vec3 m_Diffuse;
	glm::vec3 m_Specular;
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
	void ManageBasicLighting(ShaderProgramPtr shader, const LightingDesc& light, glm::vec3 objectcolor, const MaterialDesc& material);
	LightingDesc m_StudioLight;

private:

};
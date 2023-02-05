#pragma once

struct RAY_API LightingDesc
{
	glm::vec3 m_ObjectColor;
	glm::vec3 m_LightColor;
	glm::vec3 m_CameraPos;
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
	void ManageBasicLighting(ShaderProgramPtr shader, const LightingDesc& desc);

private:

};
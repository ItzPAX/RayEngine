#pragma once

struct ShaderProgramDesc
{
	const char* m_VertexShaderFilePath;
	const char* m_FragmentShaderFilePath;
};

enum class ShaderType
{
	VERTEX_SHADER = 0,
	FRAGMENT_SHADER
};

class RAY_API ShaderProgram
{
public:
	ShaderProgram(const ShaderProgramDesc& desc);
	~ShaderProgram();

	void SetUniformBufferSlot(const char* name, UINT32 slot);

private:
	void Attach(const char* shaderPath, const ShaderType& type);
	void Link();

	// getters
public:
	UINT32 GetProgramId() { return m_ProgramId; }

	void SetVec3(const char* name, glm::vec3 val);

private:
	UINT32 m_ProgramId = 0;
	UINT32 m_AttachedShaders[2] = {};
};
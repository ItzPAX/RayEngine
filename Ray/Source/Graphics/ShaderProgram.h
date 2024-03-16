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
	ShaderProgram() = default;
	~ShaderProgram();

	void SetUniformBufferSlot(const char* name, UINT32 slot);

private:
	void Attach(const char* shaderPath, const ShaderType& type);
	void Link();

	// getters
public:
	UINT32 GetProgramId() { return m_ProgramId; }

public:
	void SetVec3(const char* name, glm::vec3 val);
	void SetVec4(const char* name, glm::vec4 val);
	void SetFloat(const char* name, float val);
	void SetInt(const char* name, int val);

	std::string GetIndexedArrayUniformName(int index, const char* name);

private:
	UINT32 m_ProgramId = 0;
	UINT32 m_AttachedShaders[2] = {};

	const char* m_FragmentPath;
	const char* m_VertexPath;
};
#pragma once

struct ShaderProgramDesc
{
	const wchar_t* m_VertexShaderFilePath;
	const wchar_t* m_FragmentShaderFilePath;
};

enum ShaderType
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
	void Attach(const wchar_t* shaderPath, const ShaderType& type);
	void Link();

	// getters
public:
	UINT32 GetProgramId() { return m_ProgramId; }

private:
	UINT32 m_ProgramId = 0;
	UINT32 m_AttachedShaders[2] = {};
};
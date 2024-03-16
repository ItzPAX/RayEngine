#include "Ray.h"

ShaderProgram::ShaderProgram(const ShaderProgramDesc& desc)
{
	m_ProgramId = glCreateProgram();
	Attach(desc.m_VertexShaderFilePath, ShaderType::VERTEX_SHADER);
	Attach(desc.m_FragmentShaderFilePath, ShaderType::FRAGMENT_SHADER);
	Link();

	m_FragmentPath = desc.m_FragmentShaderFilePath;
	m_VertexPath = desc.m_VertexShaderFilePath;
}

ShaderProgram::~ShaderProgram()
{
	for (int i = 0; i < 2; i++)
	{
		glDetachShader(m_ProgramId, m_AttachedShaders[i]);
		glDeleteShader(m_AttachedShaders[i]);
	}
	glDeleteProgram(m_ProgramId);
}

void ShaderProgram::SetUniformBufferSlot(const char* name, UINT32 slot)
{
	UINT32 index = glGetUniformBlockIndex(m_ProgramId, name);
	glUniformBlockBinding(m_ProgramId, index, slot);
}

void ShaderProgram::SetVec3(const char* name, glm::vec3 val)
{
	UINT32 index = glGetUniformLocation(m_ProgramId, name);
	if (index == -1)
		return;

	glUniform3f(index, val.x, val.y, val.z);
}

void ShaderProgram::SetFloat(const char* name, float val)
{
	UINT32 index = glGetUniformLocation(m_ProgramId, name);
	if (index == -1)
		return;

	glUniform1f(index, val);
}

void ShaderProgram::Attach(const char* shaderPath, const ShaderType& type)
{
	std::string shaderCode;

	std::ifstream shaderStream(shaderPath);
	if (shaderStream.is_open())
	{
		std::stringstream sstr;
		sstr << shaderStream.rdbuf();
		shaderCode = sstr.str();
		shaderStream.close();
	}
	else
	{
		std::wstringstream ss;
		ss << L"ShaderProgram->" << shaderPath << " not found";
		Logger::PrintOGL3DWarning(ss.str().c_str());
		return;
	}

	UINT32 shaderId = 0;
	if (type == ShaderType::VERTEX_SHADER)
		shaderId = glCreateShader(GL_VERTEX_SHADER);
	else if (type == ShaderType::FRAGMENT_SHADER)
		shaderId = glCreateShader(GL_FRAGMENT_SHADER);

	auto sourcePointer = shaderCode.c_str();
	glShaderSource(shaderId, 1, &sourcePointer, NULL);
	glCompileShader(shaderId);

	// get compile errors
	INT logLength = 0;
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		std::vector<char> errorMessage(logLength + 1);
		glGetShaderInfoLog(shaderId, logLength, NULL, &errorMessage[0]);

		wchar_t* wc = new wchar_t[errorMessage.size()];
		mbstowcs(wc, &errorMessage[0], errorMessage.size());

		std::wstringstream ss;
		ss << "ShaderProgram->" << shaderPath << " couldn't compile: " << wc;

		Logger::PrintOGL3DWarning(ss.str().c_str());
	}

	glAttachShader(m_ProgramId, shaderId);
	m_AttachedShaders[(UINT32)type] = shaderId;
}

void ShaderProgram::Link()
{
	glLinkProgram(m_ProgramId);

	// get compile errors
	INT logLength = 0;
	glGetShaderiv(m_ProgramId, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		std::vector<char> errorMessage(logLength + 1);
		glGetShaderInfoLog(m_ProgramId, logLength, NULL, &errorMessage[0]);

		wchar_t* wc = new wchar_t[errorMessage.size()];
		mbstowcs(wc, &errorMessage[0], errorMessage.size());

		std::wstringstream ss;
		ss << "ShaderProgram->" << wc;

		Logger::PrintOGL3DWarning(ss.str().c_str());
	}
}
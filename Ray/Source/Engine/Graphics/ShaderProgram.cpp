#include "Ray.h"

ShaderProgram::ShaderProgram(const ShaderProgramDesc& desc)
{
	m_ProgramId = glCreateProgram();
	Attach(desc.m_VertexShaderFilePath, VERTEX_SHADER);
	Attach(desc.m_FragmentShaderFilePath, FRAGMENT_SHADER);
	Link();
}

ShaderProgram::~ShaderProgram()
{
	for (int i = 0; i < 2; ++i)
	{
		glDetachShader(m_ProgramId, m_AttachedShaders[i]);
		glDeleteShader(m_AttachedShaders[i]);
	}
	glDeleteProgram(m_ProgramId);
}

void ShaderProgram::Attach(const wchar_t* shaderPath, const ShaderType& type)
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
		ss << L"ShaderProgram->" << shaderPath << " not found" << std::endl;
		Logger::PrintOGL3DWarning(ss.str().c_str());
		return;
	}

	UINT32 shaderId = 0;
	if (type == VERTEX_SHADER)
		shaderId = glCreateShader(GL_VERTEX_SHADER);
	else if (type == FRAGMENT_SHADER)
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
		ss << "ShaderProgram->" << shaderPath << " couldn't compile: " << std::endl << wc;

		Logger::PrintOGL3DWarning(ss.str().c_str());
	}

	glAttachShader(m_ProgramId, shaderId);
	m_AttachedShaders[type] = shaderId;
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
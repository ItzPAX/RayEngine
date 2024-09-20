#include "Ray.h"
#include <stb_image/stb_image.h>

Texture::Texture(const char* path, const char* type, bool flip)
{
	glGenTextures(1, &m_TextureID);

	stbi_set_flip_vertically_on_load(flip);

	m_Data = stbi_load(path, &m_Width, &m_Height, &m_Channels, 0);
	if (m_Data)
	{
		m_Format = GL_RGB;
		if (m_Channels == 1)
			m_Format = GL_RED;
		else if (m_Channels == 3)
			m_Format = GL_RGB;
		else if (m_Channels == 4)
			m_Format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, m_Format, m_Width, m_Height, 0, m_Format, GL_UNSIGNED_BYTE, m_Data);
		glGenerateMipmap(GL_TEXTURE_2D);

		m_Type = type;

		std::wstringstream ss;
		ss << L"Texture->Created: " << path << L" Channels: " << std::to_wstring(m_Channels) << L" Type: " << type;
		Logger::PrintOGL3DInfo(ss.str().c_str());
	}
	else
	{
		std::wstringstream ss;
		ss << L"Texture->Couldn't create: " << path << "\n" << stbi_failure_reason();
		Logger::PrintOGL3DWarning(ss.str().c_str());
	}

	m_Path = std::string(path);
	glBindTexture(GL_TEXTURE_2D, NULL);
}

Texture::~Texture()
{
	/*TODO TODO Fix crash here*/
	//stbi_image_free(m_Data);
	glDeleteTextures(1, &m_TextureID);
}

void Texture::SetFiltering(UINT32 mode)
{
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);

	glBindTexture(GL_TEXTURE_2D, NULL);
}

void Texture::Delete()
{
	/*TODO TODO Fix crash here*/
	//stbi_image_free(m_Data);
	glDeleteTextures(1, &m_TextureID);
}

void Texture::SetWrapping(UINT32 mode)
{
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	if (mode == GL_CLAMP_TO_BORDER)
	{
		float borderColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode);

	glBindTexture(GL_TEXTURE_2D, NULL);
}
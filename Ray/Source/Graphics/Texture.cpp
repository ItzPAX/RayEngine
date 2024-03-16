#include "Ray.h"
#include <stb_image/stb_image.h>

Texture::Texture(const char* path)
{
	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	stbi_set_flip_vertically_on_load(true);

	m_Data = stbi_load(path, &m_Width, &m_Height, &m_Channels, 4);
	if (m_Data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::wstringstream ss;
		ss << L"Couldnt load texture " << path;
		Logger::PrintOGL3DWarning(ss.str().c_str());
	}

	glBindTexture(GL_TEXTURE_2D, NULL);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_TextureID);
	stbi_image_free(m_Data);
}


void Texture::SetFiltering(UINT32 mode)
{
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mode);

	glBindTexture(GL_TEXTURE_2D, NULL);
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
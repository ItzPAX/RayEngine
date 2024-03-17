#pragma once

class RAY_API Texture
{
public:
	Texture(const char* path, bool flip = true);
	~Texture();

	void Delete();
	void SetWrapping(UINT32 mode);
	void SetFiltering(UINT32 mode);

	UINT32 GetTextureID() { return m_TextureID; }

private:
	INT m_Width;
	INT m_Height;
	INT m_Channels;
	UINT32 m_TextureID;
	unsigned int m_Format;

	UCHAR* m_Data;

public:
	std::string m_Type;
	std::string m_Path;
};
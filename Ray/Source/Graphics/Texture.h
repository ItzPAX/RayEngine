#pragma once

class RAY_API Texture
{
public:
	Texture(const char* path);
	~Texture();

	void SetWrapping(UINT32 mode);
	void SetFiltering(UINT32 mode);

	UINT32 GetTextureID() { return m_TextureID; }

private:
	INT m_Width;
	INT m_Height;
	INT m_Channels;
	UINT32 m_TextureID;

	UCHAR* m_Data;
};
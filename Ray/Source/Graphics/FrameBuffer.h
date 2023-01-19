#pragma once

class RAY_API FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Bind();
	void Unbind();

	// getters
public:
	UINT32 Textures() { return m_Textures[0]; }
	UINT32 GetID() { return m_FboId; }
	glm::vec2 Size() { return m_Size; }

private:
	glm::vec2 m_Size;
	UINT32 m_FboId;
	UINT32 m_Textures[2]; // COLOR | DEPTH Buffers
};
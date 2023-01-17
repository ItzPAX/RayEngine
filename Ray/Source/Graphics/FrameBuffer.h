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
	Vec2D Size() { return m_Size; }

private:
	Vec2D m_Size;
	UINT32 m_FboId;
	UINT32 m_Textures[2]; // COLOR | DEPTH Buffers
};
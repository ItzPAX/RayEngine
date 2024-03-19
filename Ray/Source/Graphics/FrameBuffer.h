#pragma once
#include "glad/glad.h"

struct RAY_API FrameBufferDescription
{
	GLenum internalformats[2];
	GLenum formats[2];
	GLenum types[2];

	FrameBufferDescription()
	{
		internalformats[0] = GL_RGBA; internalformats[1] = GL_DEPTH24_STENCIL8;
		formats[0] = GL_RGBA; formats[1] = GL_DEPTH_STENCIL;
		types[0] = GL_UNSIGNED_BYTE; types[1] = GL_UNSIGNED_INT_24_8;
	}
};

class RAY_API FrameBuffer
{
public:
	FrameBuffer(const FrameBufferDescription& fbdesc);
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
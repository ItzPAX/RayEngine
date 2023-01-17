#include "Ray.h"

FrameBuffer::FrameBuffer(Vec2D size)
{
	glGenFramebuffers(1, &m_FboId);
	glGenTextures(2, m_Textures);

	m_Size = size;
	
	glBindTexture(GL_TEXTURE_2D, m_Textures[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)size.x(), (GLsizei)size.y(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, m_Textures[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, (GLsizei)size.x(), (GLsizei)size.y(), 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, NULL);

	Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_Textures[0], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_Textures[1], 0);
	Unbind();
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_FboId);
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FboId);
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, NULL);
}
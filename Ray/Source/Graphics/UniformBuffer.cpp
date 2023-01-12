#include "Ray.h"

UniformBuffer::UniformBuffer(const UniformBufferDesc& desc)
{
	glGenBuffers(1, &m_BufferId);
	glBindBuffer(GL_UNIFORM_BUFFER, m_BufferId);
	glBufferData(GL_UNIFORM_BUFFER, desc.m_Size, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, NULL);
	m_BufferSize = desc.m_Size;
}

UniformBuffer::~UniformBuffer()
{
	glDeleteBuffers(1, &m_BufferId);
}

void UniformBuffer::SetData(void* data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_BufferId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, m_BufferSize, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
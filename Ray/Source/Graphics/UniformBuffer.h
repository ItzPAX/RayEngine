#pragma once

struct RAY_API VertexData
{
	glm::mat4 u_ModelViewProj;
	glm::mat4 u_Model;
	glm::vec3 u_ObjectColor;
};

struct UniformBufferDesc
{
	UINT32 m_Size = 0;
};

class RAY_API UniformBuffer
{
public:
	UniformBuffer(const UniformBufferDesc& desc);
	~UniformBuffer();

	void SetData(void* data);

	// getters
public:
	UINT32 GetBufferId() { return m_BufferId; }

private:
	UINT32 m_BufferId = 0;
	UINT32 m_BufferSize = 0;
};
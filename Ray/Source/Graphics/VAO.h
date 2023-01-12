#pragma once

struct VertexAttribute
{
	UINT32 m_NumElements = 0;
};

struct VertexBufferDesc
{
	void* m_VerticesList = nullptr;
	UINT32 m_VertexSize = 0;
	UINT32 m_ListSize = 0;

	VertexAttribute* m_AttributeList = nullptr;
	UINT32 m_AttributeListSize;
};

struct IndexBufferDesc
{
	void* m_IndicesList = nullptr;
	UINT32 m_ListSize = 0;
};

class RAY_API VertexArrayObject
{
public:
	VertexArrayObject(const VertexBufferDesc& vbDesc);
	VertexArrayObject(const VertexBufferDesc& vbDesc, const IndexBufferDesc& ibDesc);
	~VertexArrayObject();

public:
	UINT32 GetVAOId() { return m_VertexArrayObjectId; };
	UINT32 GetVertexBufferSize() { return m_VertexBufferData.m_ListSize; }
	UINT32 GetVertexSize() { return m_VertexBufferData.m_VertexSize; }

private:
	UINT32 m_VertexBufferId = 0;
	UINT32 m_ElementBufferId = 0;
	UINT32 m_VertexArrayObjectId = 0;
	VertexBufferDesc m_VertexBufferData;
};
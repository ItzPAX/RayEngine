#include "Ray.h"

VertexArrayObject::VertexArrayObject(const VertexBufferDesc& vbDesc)
{
	if (!vbDesc.m_ListSize)
		Logger::PrintOGL3DError(L"VertexArrayObject->m_ListSize is NULL");
	if (!vbDesc.m_VerticesList)
		Logger::PrintOGL3DError(L"VertexArrayObject->m_VerticesList is NULL");
	if (!vbDesc.m_AttributeList)
		Logger::PrintOGL3DError(L"VertexArrayObject->m_AttributeList is NULL");

	glGenVertexArrays(1, &m_VertexArrayObjectId);
	glBindVertexArray(m_VertexArrayObjectId);

	glGenBuffers(1, &m_VertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, vbDesc.m_ListSize * vbDesc.m_VertexSize, vbDesc.m_VerticesList, GL_STATIC_DRAW);

	int elements = 0;
	for (unsigned int i = 0; i < vbDesc.m_AttributeListSize; i++)
	{
		glVertexAttribPointer
		(
			i,
			vbDesc.m_AttributeList[i].m_NumElements,
			GL_FLOAT,
			GL_FALSE,
			vbDesc.m_VertexSize,
			(void*)elements
		);
		elements += vbDesc.m_AttributeList[i].m_NumElements * sizeof(float);
		glEnableVertexAttribArray(i);
	}

	glBindVertexArray(NULL);

	m_VertexBufferData = vbDesc;
}

VertexArrayObject::VertexArrayObject(const VertexBufferDesc& vbDesc, const IndexBufferDesc& ibDesc) 
	: VertexArrayObject(vbDesc)
{
	if (!ibDesc.m_ListSize)
		Logger::PrintOGL3DError(L"VertexArrayObject->m_ListSize is NULL");
	if (!ibDesc.m_IndicesList)
		Logger::PrintOGL3DError(L"VertexArrayObject->m_IndicesList is NULL");

	glBindVertexArray(m_VertexArrayObjectId);

	glGenBuffers(1, &m_ElementBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ElementBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibDesc.m_ListSize, ibDesc.m_IndicesList, GL_STATIC_DRAW);

	glBindVertexArray(NULL);
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteBuffers(1, &m_ElementBufferId);
	glDeleteBuffers(1, &m_VertexBufferId);
	glDeleteVertexArrays(1, &m_VertexArrayObjectId);
}

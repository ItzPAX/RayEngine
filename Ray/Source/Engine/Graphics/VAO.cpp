#include "Ray.h"

VertexArrayObject::VertexArrayObject(const VertexBufferDesc& data)
{
	if (!data.m_ListSize)
		Logger::PrintOGL3DError(L"VertexArrayObject->m_ListSize is NULL");
	if (!data.m_VerticesList)
		Logger::PrintOGL3DError(L"VertexArrayObject->m_VerticesList is NULL");
	if (!data.m_AttributeList)
		Logger::PrintOGL3DError(L"VertexArrayObject->m_AttributeList is NULL");

	glGenVertexArrays(1, &m_VertexArrayObjectId);
	glBindVertexArray(m_VertexArrayObjectId);

	glGenBuffers(1, &m_VertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, data.m_ListSize * data.m_VertexSize, data.m_VerticesList, GL_STATIC_DRAW);

	for (int i = 0; i < data.m_AttributeListSize; i++)
	{
		glVertexAttribPointer
		(
			i, 
			data.m_AttributeList[i].m_NumElements,
			GL_FLOAT, 
			GL_FALSE, 
			data.m_VertexSize, 
			(void*)((i == 0) ? 0 : data.m_AttributeList[i - 1].m_NumElements * sizeof(float))
		);
		glEnableVertexAttribArray(i);
	}

	glBindVertexArray(NULL);

	m_VertexBufferData = data;
}

VertexArrayObject::~VertexArrayObject()
{
	glDeleteBuffers(1, &m_VertexBufferId);
	glDeleteVertexArrays(1, &m_VertexArrayObjectId);
}

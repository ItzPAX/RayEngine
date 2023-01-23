#include "Ray.h"

void PrimitiveContainer::Render(glm::mat4 viewproj)
{
	// render all elements
	for (auto p : m_Pyramids)
		p.Render(viewproj);

	for (auto p : m_Cubes)
		p.Render(viewproj);

	for (auto p : m_Squares)
		p.Render(viewproj);

	for (auto p : m_Triangles)
		p.Render(viewproj);
}


Primitive::Primitive(const PrimitiveDesc& desc)
{
	m_Description = desc;
	m_Model = glm::mat4(1.f);
	m_Model = glm::translate(m_Model, m_Description.m_Pos);

	m_UniformBuffer = Graphics::Instance()->CreateUniformBuffer({sizeof(UniformData)});

	if (m_Description.m_Texture)
	{
		m_Texture = Graphics::Instance()->CreateTexture(m_Description.m_Texture);
	}
	else
	{
		Logger::PrintOGL3DWarning(L"No texture provided for primitive\n");
	}

	if (m_Description.m_VertexShader && m_Description.m_FragmentShader)
	{
		m_Shader = Graphics::Instance()->CreateShaderProgram
		({
			m_Description.m_VertexShader,
			m_Description.m_FragmentShader
			});

		m_Shader->SetUniformBufferSlot("EngineData", 0);
	}
	else
	{
		Logger::PrintOGL3DWarning(L"No shader info provided for primitive\n");
	}
}

Primitive::~Primitive()
{
}

void Primitive::Render(glm::mat4 viewproj)
{
	UniformData data = { viewproj * m_Model };
	m_UniformBuffer->SetData(&data);

	if (m_Texture)
		Graphics::Instance()->SetTexture(m_Texture);

	Graphics::Instance()->SetVertexArrayObject(m_VAO);

	Graphics::Instance()->SetUniformBuffer(m_UniformBuffer, 0);

	if (m_Shader)
		Graphics::Instance()->SetShaderProgram(m_Shader);
}

Square::Square(const PrimitiveDesc& desc)
	: Primitive(desc)
{
	Vertex vertices[] =
	{
		{ glm::vec3(0.5f, 0.5f, 0.f),   glm::vec2(1.f, 1.f),  m_Description.m_Col },
		{ glm::vec3(0.5f, -0.5f, 0.f),  glm::vec2(1.f, 0.f),  m_Description.m_Col },
		{ glm::vec3(-0.5f, -0.5f, 0.f), glm::vec2(0.f, 0.f),  m_Description.m_Col },
		{ glm::vec3(-0.5f, 0.5f, 0.f),  glm::vec2(0.f, 1.f),  m_Description.m_Col }
	};

	UINT32 indices[] =
	{
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	VertexAttribute attribList[] =
	{
		sizeof(glm::vec3) / sizeof(float),		// POSITION
		sizeof(glm::vec2) / sizeof(float),		// TEXCOORD
		sizeof(glm::vec3) / sizeof(float)		// COLOR
	};

	m_VAO = Graphics::Instance()->CreateVertexArrayObject
	(
		{
			vertices,
			sizeof(Vertex),
			sizeof(vertices) / sizeof(Vertex),

			attribList,
			sizeof(attribList) / sizeof(VertexAttribute)
		},
		{
			indices,
			sizeof(indices)
		}
	);

	PrimitiveContainer::Instance().Add(*this);
}

void Square::Render(glm::mat4 viewproj)
{
	Primitive::Render(viewproj);
	Graphics::Instance()->DrawIndexedTriangles(TriangleType::TRIANGLE_LIST, 6);
}

Triangle::Triangle(const PrimitiveDesc& desc)
	: Primitive(desc)
{
	Vertex vertices[] =
	{
		{ glm::vec3(-0.5f, -0.5f, 0.f), glm::vec2(0.f, 0.f),  m_Description.m_Col },
		{ glm::vec3(0.5f, -0.5f, 0.f),  glm::vec2(1.f, 0.f),  m_Description.m_Col },
		{ glm::vec3(0.f, 0.5f, 0.f),    glm::vec2(0.5f, 1.f), m_Description.m_Col }
	};

	VertexAttribute attribList[] =
	{
		sizeof(glm::vec3) / sizeof(float),		// POSITION
		sizeof(glm::vec2) / sizeof(float),		// TEXCOORD
		sizeof(glm::vec3) / sizeof(float)		// COLOR
	};

	m_VAO = Graphics::Instance()->CreateVertexArrayObject
	(
		{
			vertices,
			sizeof(Vertex),
			sizeof(vertices) / sizeof(Vertex),

			attribList,
			sizeof(attribList) / sizeof(VertexAttribute)
		}
	);

	PrimitiveContainer::Instance().Add(*this);
}

void Triangle::Render(glm::mat4 viewproj)
{
	Primitive::Render(viewproj);
	Graphics::Instance()->DrawTriangles(TriangleType::TRIANGLE_LIST, 3, 0);
}

Cube::Cube(const PrimitiveDesc& desc)
	: Primitive(desc)
{
	glm::vec3 positionsList[] =
	{
		//front face
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f,0.5f,-0.5f),
		glm::vec3(0.5f,0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),

		//back face
		glm::vec3(0.5f,-0.5f,0.5f),
		glm::vec3(0.5f,0.5f,0.5f),
		glm::vec3(-0.5f,0.5f,0.5f),
		glm::vec3(-0.5f,-0.5f,0.5f)
	};

	glm::vec2 texcoordsList[] =
	{
		glm::vec2(1,1),
		glm::vec2(1,0),
		glm::vec2(0,1),
		glm::vec2(0,0)
	};

	Vertex verticesList[] =
	{
		//front face
		{ positionsList[0],texcoordsList[1], m_Description.m_Col },
		{ positionsList[1],texcoordsList[0], m_Description.m_Col },
		{ positionsList[2],texcoordsList[2], m_Description.m_Col },
		{ positionsList[3],texcoordsList[3], m_Description.m_Col },

		//back face
		{ positionsList[4],texcoordsList[1], m_Description.m_Col },
		{ positionsList[5],texcoordsList[0], m_Description.m_Col },
		{ positionsList[6],texcoordsList[2], m_Description.m_Col },
		{ positionsList[7],texcoordsList[3], m_Description.m_Col },

		//top face
		{ positionsList[1],texcoordsList[1], m_Description.m_Col },
		{ positionsList[6],texcoordsList[0], m_Description.m_Col },
		{ positionsList[5],texcoordsList[2], m_Description.m_Col },
		{ positionsList[2],texcoordsList[3], m_Description.m_Col },

		//bottom face
		{ positionsList[7],texcoordsList[1], m_Description.m_Col },
		{ positionsList[0],texcoordsList[0], m_Description.m_Col },
		{ positionsList[3],texcoordsList[2], m_Description.m_Col },
		{ positionsList[4],texcoordsList[3], m_Description.m_Col },

		//right face
		{ positionsList[3],texcoordsList[1], m_Description.m_Col },
		{ positionsList[2],texcoordsList[0], m_Description.m_Col },
		{ positionsList[5],texcoordsList[2], m_Description.m_Col },
		{ positionsList[4],texcoordsList[3], m_Description.m_Col },

		//left face
		{ positionsList[7],texcoordsList[1], m_Description.m_Col },
		{ positionsList[6],texcoordsList[0], m_Description.m_Col },
		{ positionsList[1],texcoordsList[2], m_Description.m_Col },
		{ positionsList[0],texcoordsList[3], m_Description.m_Col }
	};

	UINT32 indicesList[] =
	{
		//front
		0,1,2,
		2,3,0,

		//back
		4,5,6,
		6,7,4,

		//top
		8,9,10,
		10,11,8,

		//bottom
		12,13,14,
		14,15,12,

		//right
		16,17,18,
		18,19,16,

		//left
		20,21,22,
		22,23,20
	};

	VertexAttribute attribList[] =
	{
		sizeof(glm::vec3) / sizeof(float),		// POSITION
		sizeof(glm::vec2) / sizeof(float),		// TEXCOORD
		sizeof(glm::vec3) / sizeof(float)		// COLOR
	};

	m_VAO = Graphics::Instance()->CreateVertexArrayObject
	(
		{
			(void*)verticesList,
			sizeof(Vertex),
			sizeof(verticesList) / sizeof(Vertex),

			attribList,
			sizeof(attribList) / sizeof(VertexAttribute)
		},
		{
			(void*)indicesList,
			sizeof(indicesList)
		}
	);

	PrimitiveContainer::Instance().Add(*this);
}

void Cube::Render(glm::mat4 viewproj)
{
	Primitive::Render(viewproj);
	Graphics::Instance()->DrawIndexedTriangles(TriangleType::TRIANGLE_LIST, 36);
}

Pyramid::Pyramid(const PrimitiveDesc& desc)
	: Primitive(desc)
{
	glm::vec3 top(0.f, 0.5f, 0.f);
	glm::vec3 p01(-0.5f, -0.5f, 0.5f);
	glm::vec3 p11(0.5f, -0.5f, 0.5f);
	glm::vec3 p00(-0.5f, -0.5f, -0.5f);
	glm::vec3 p10(0.5f, -0.5f, -0.5f);

	Vertex verticesList[]
	{
		{ p00, glm::vec2(0.0f, 0.0f), m_Description.m_Col },
		{ p10, glm::vec2(1.0f, 0.0f), m_Description.m_Col },
		{ p11, glm::vec2(1.0f, 1.0f), m_Description.m_Col },
		{ p00, glm::vec2(0.0f, 0.0f), m_Description.m_Col },
		{ p11, glm::vec2(1.0f, 1.0f), m_Description.m_Col },
		{ p01, glm::vec2(0.0f, 1.0f), m_Description.m_Col },

		{ p00, glm::vec2(0.0f, 0.0f), m_Description.m_Col },
		{ p10, glm::vec2(1.0f, 0.0f), m_Description.m_Col },
		{ top, glm::vec2(0.5f, 1.0f), m_Description.m_Col },

		{ p10, glm::vec2(0.0f, 0.0f), m_Description.m_Col },
		{ p11, glm::vec2(1.0f, 0.0f), m_Description.m_Col },
		{ top, glm::vec2(0.5f, 1.0f), m_Description.m_Col },

		{ p11, glm::vec2(0.0f, 0.0f), m_Description.m_Col },
		{ p01, glm::vec2(1.0f, 0.0f), m_Description.m_Col },
		{ top, glm::vec2(0.5f, 1.0f), m_Description.m_Col },

		{ p01, glm::vec2(0.0f, 0.0f), m_Description.m_Col },
		{ p00, glm::vec2(1.0f, 0.0f), m_Description.m_Col },
		{ top, glm::vec2(0.5f, 1.0f), m_Description.m_Col }
	};

	VertexAttribute attribList[] =
	{
		sizeof(glm::vec3) / sizeof(float),		// POSITION
		sizeof(glm::vec2) / sizeof(float),		// TEXCOORD
		sizeof(glm::vec3) / sizeof(float)		// COLOR
	};

	m_VAO = Graphics::Instance()->CreateVertexArrayObject
	(
		{
			(void*)verticesList,
			sizeof(Vertex),
			sizeof(verticesList) / sizeof(Vertex),

			attribList,
			sizeof(attribList) / sizeof(VertexAttribute)
		}
	);

	PrimitiveContainer::Instance().Add(*this);
}

void Pyramid::Render(glm::mat4 viewproj)
{
	Primitive::Render(viewproj);
	Graphics::Instance()->DrawTriangles(TriangleType::TRIANGLE_LIST, 18, 0);
}
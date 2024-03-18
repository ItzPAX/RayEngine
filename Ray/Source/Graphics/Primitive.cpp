#include "Ray.h"

void PrimitiveContainer::Render(float dt)
{
	// render all elements
	for (auto& p : m_Primitives)
	{
		switch (p.second.m_Type)
		{
		case PRIMITIVE_TYPE::PRIMITIVE_CUBE:
			((Cube*)p.second.m_Data)->Render(dt);
			break;
		case PRIMITIVE_TYPE::PRIMITIVE_PYRAMID:
			((Pyramid*)p.second.m_Data)->Render(dt);
			break;
		case PRIMITIVE_TYPE::PRIMITIVE_SQUARE:
			((Square*)p.second.m_Data)->Render(dt);
			break;
		case PRIMITIVE_TYPE::PRIMITIVE_TRIANGLE:
			((Triangle*)p.second.m_Data)->Render(dt);
			break;
		}
	}
}

void Primitive::UpdatePrimitive(float dt)
{
	m_Description.m_Rotation += (m_Description.m_RotationVel * dt);
	m_Description.m_Position += (m_Description.m_Velocity * dt);

	m_Model = glm::mat4(1.0f);

	m_Model = glm::translate(m_Model, m_Description.m_Position);

	m_Model = glm::rotate(m_Model, m_Description.m_Rotation.x, glm::vec3(1, 0, 0));
	m_Model = glm::rotate(m_Model, m_Description.m_Rotation.y, glm::vec3(0, 1, 0));
	m_Model = glm::rotate(m_Model, m_Description.m_Rotation.z, glm::vec3(0, 0, 1));
}

void Primitive::Delete()
{
	PrimitiveContainer::Instance().m_Primitives.erase(m_Index);
}

Primitive::Primitive(const PrimitiveDesc& desc, const MaterialDesc& material)
{
	m_Description = desc;
	m_Material = material;

	m_Model = glm::mat4(1.f);

	m_UniformBuffer = Graphics::Instance()->CreateUniformBuffer({ sizeof(VertexData) });

	m_Texture = Graphics::Instance()->CreateTexture(m_Description.m_Texture, "diffuse_texture");

	m_Shader = Graphics::Instance()->CreateShaderProgram
	({
		m_Description.m_VertexShader,
		m_Description.m_FragmentShader
	});

	m_Shader->SetUniformBufferSlot("VertexData", 0);

	m_Index = PrimitiveContainer::Instance().m_Primitives.size();
}

void Primitive::Render(float dt)
{
	UpdatePrimitive(dt);

	glm::mat4 scaling_mat = glm::mat4(1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f);

	VertexData vdata = { FloatingCamera::GetFloatingCam().GetViewProj() * m_Model, m_Model, scaling_mat };
	m_UniformBuffer->SetData(&vdata);

	Graphics::Instance()->SetTexture(m_Texture);
	Graphics::Instance()->SetVertexArrayObject(m_VAO);
		
	Graphics::Instance()->SetShaderProgram(m_Shader);
	Graphics::Instance()->SetUniformBuffer(m_UniformBuffer, 0);
	LightingManager::Instance().ManageBasicLighting(m_Shader, m_Description.m_ObjectColor, m_Material);
}

glm::vec3 Primitive::GetTranslationScale()
{
	return m_Description.m_Velocity;
}

glm::vec3 Primitive::GetRotationScale()
{
	return m_Description.m_RotationVel;
}

void Primitive::SetTranslationScale(glm::vec3 s)
{
	m_Description.m_Velocity = s;
}

void Primitive::SetRotationScale(glm::vec3 s)
{
	m_Description.m_RotationVel = s;
}

PrimitiveDesc* Primitive::GetDescription()
{
	return &m_Description;
}

MaterialDesc* Primitive::GetMaterial()
{
	return &m_Material;
}

PRIMITIVE_TYPE Primitive::GetType()
{
	return m_Type;
}

Square::Square(const PrimitiveDesc& desc, const MaterialDesc& material)
	: Primitive(desc, material)
{
	Vertex vertices[] =
	{
		{ glm::vec3(0.5f, 0.5f, 0.f),   glm::vec2(1.f, 1.f) },
		{ glm::vec3(0.5f, -0.5f, 0.f),  glm::vec2(1.f, 0.f) },
		{ glm::vec3(-0.5f, -0.5f, 0.f), glm::vec2(0.f, 0.f) },
		{ glm::vec3(-0.5f, 0.5f, 0.f),  glm::vec2(0.f, 1.f) }
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

	m_Type = PRIMITIVE_TYPE::PRIMITIVE_SQUARE;
}

void Square::Render(float dt)
{
	Primitive::Render(dt);
	Graphics::Instance()->DrawIndexedTriangles(TriangleType::TRIANGLE_LIST, 6);
}

Triangle::Triangle(const PrimitiveDesc& desc, const MaterialDesc& material)
	: Primitive(desc, material)
{
	Vertex vertices[] =
	{
		{ glm::vec3(-0.5f, -0.5f, 0.f), glm::vec2(0.f, 0.f) },
		{ glm::vec3(0.5f, -0.5f, 0.f),  glm::vec2(1.f, 0.f) },
		{ glm::vec3(0.f, 0.5f, 0.f),    glm::vec2(0.5f, 1.f) }
	};

	VertexAttribute attribList[] =
	{
		sizeof(glm::vec3) / sizeof(float),		// POSITION
		sizeof(glm::vec2) / sizeof(float),		// TEXCOORD
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

	m_Type = PRIMITIVE_TYPE::PRIMITIVE_TRIANGLE;
}

void Triangle::Render(float dt)
{
	Primitive::Render(dt);
	Graphics::Instance()->DrawTriangles(TriangleType::TRIANGLE_LIST, 3, 0);
}

Cube::Cube(const PrimitiveDesc& desc, const MaterialDesc& material)
	: Primitive(desc, material)
{
	glm::vec3 positionsList[] =
	{
		//FRONT
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f,0.5f,-0.5f),
		glm::vec3(0.5f,0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),

		//BACK
		glm::vec3(0.5f,-0.5f,0.5f),
		glm::vec3(0.5f,0.5f,0.5f),
		glm::vec3(-0.5f,0.5f,0.5f),
		glm::vec3(-0.5f,-0.5f,0.5f)
	};


	glm::vec2 texcoordsList[] =
	{
		glm::vec2(0,0),
		glm::vec2(0,1),
		glm::vec2(1,0),
		glm::vec2(1,1)
	};

	Vertex verticesList[] =
	{
		//front face
		{ positionsList[0],texcoordsList[1], glm::vec3(0.f, 0.f, -1.f) },
		{ positionsList[1],texcoordsList[0], glm::vec3(0.f, 0.f, -1.f) },
		{ positionsList[2],texcoordsList[2], glm::vec3(0.f, 0.f, -1.f) },
		{ positionsList[3],texcoordsList[3], glm::vec3(0.f, 0.f, -1.f) },

		//back face
		{ positionsList[4],texcoordsList[1], glm::vec3(0.f, 0.f, 1.f) },
		{ positionsList[5],texcoordsList[0], glm::vec3(0.f, 0.f, 1.f) },
		{ positionsList[6],texcoordsList[2], glm::vec3(0.f, 0.f, 1.f) },
		{ positionsList[7],texcoordsList[3], glm::vec3(0.f, 0.f, 1.f) },

		//top face
		{ positionsList[1],texcoordsList[1], glm::vec3(0.f, 1.f, 0.f) },
		{ positionsList[6],texcoordsList[0], glm::vec3(0.f, 1.f, 0.f) },
		{ positionsList[5],texcoordsList[2], glm::vec3(0.f, 1.f, 0.f) },
		{ positionsList[2],texcoordsList[3], glm::vec3(0.f, 1.f, 0.f) },

		//bottom face
		{ positionsList[7],texcoordsList[1], glm::vec3(0.f, -1.f, 0.f) },
		{ positionsList[0],texcoordsList[0], glm::vec3(0.f, -1.f, 0.f) },
		{ positionsList[3],texcoordsList[2], glm::vec3(0.f, -1.f, 0.f) },
		{ positionsList[4],texcoordsList[3], glm::vec3(0.f, -1.f, 0.f) },

		//right face
		{ positionsList[3],texcoordsList[1], glm::vec3(1.f, 0.f, 0.f) },
		{ positionsList[2],texcoordsList[0], glm::vec3(1.f, 0.f, 0.f) },
		{ positionsList[5],texcoordsList[2], glm::vec3(1.f, 0.f, 0.f) },
		{ positionsList[4],texcoordsList[3], glm::vec3(1.f, 0.f, 0.f) },

		//left face
		{ positionsList[7],texcoordsList[1], glm::vec3(-1.f, 0.f, 0.f) },
		{ positionsList[6],texcoordsList[0], glm::vec3(-1.f, 0.f, 0.f) },
		{ positionsList[1],texcoordsList[2], glm::vec3(-1.f, 0.f, 0.f) },
		{ positionsList[0],texcoordsList[3], glm::vec3(-1.f, 0.f, 0.f) }
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
		sizeof(glm::vec3) / sizeof(float)		// NORMAL
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

	m_Type = PRIMITIVE_TYPE::PRIMITIVE_CUBE;
}

void Cube::Render(float dt)
{
	Primitive::Render(dt);
	Graphics::Instance()->DrawIndexedTriangles(TriangleType::TRIANGLE_LIST, 36);
}

Pyramid::Pyramid(const PrimitiveDesc& desc, const MaterialDesc& material)
	: Primitive(desc, material)
{
	glm::vec3 top(0.f, 0.5f, 0.f);
	glm::vec3 p01(-0.5f, -0.5f, 0.5f);
	glm::vec3 p11(0.5f, -0.5f, 0.5f);
	glm::vec3 p00(-0.5f, -0.5f, -0.5f);
	glm::vec3 p10(0.5f, -0.5f, -0.5f);

	Vertex verticesList[]
	{
		{ p00, glm::vec2(0.0f, 0.0f) },
		{ p10, glm::vec2(1.0f, 0.0f) },
		{ p11, glm::vec2(1.0f, 1.0f) },
		{ p00, glm::vec2(0.0f, 0.0f) },
		{ p11, glm::vec2(1.0f, 1.0f) },
		{ p01, glm::vec2(0.0f, 1.0f) },

		{ p00, glm::vec2(0.0f, 0.0f) },
		{ p10, glm::vec2(1.0f, 0.0f) },
		{ top, glm::vec2(0.5f, 1.0f) },

		{ p10, glm::vec2(0.0f, 0.0f) },
		{ p11, glm::vec2(1.0f, 0.0f) },
		{ top, glm::vec2(0.5f, 1.0f) },

		{ p11, glm::vec2(0.0f, 0.0f) },
		{ p01, glm::vec2(1.0f, 0.0f) },
		{ top, glm::vec2(0.5f, 1.0f) },

		{ p01, glm::vec2(0.0f, 0.0f) },
		{ p00, glm::vec2(1.0f, 0.0f) },
		{ top, glm::vec2(0.5f, 1.0f) }
	};

	VertexAttribute attribList[] =
	{
		sizeof(glm::vec3) / sizeof(float),		// POSITION
		sizeof(glm::vec2) / sizeof(float),		// TEXCOORD
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

	m_Type = PRIMITIVE_TYPE::PRIMITIVE_PYRAMID;
}

void Pyramid::Render(float dt)
{
	Primitive::Render(dt);
	Graphics::Instance()->DrawTriangles(TriangleType::TRIANGLE_LIST, 18, 0);
}

std::shared_ptr<PrimitiveRenderData> PrimitiveContainer::Add(PRIMITIVE_TYPE type, const PrimitiveDesc& desc, const MaterialDesc& material)
{
	int index = m_Primitives.size();

	switch (type)
	{
	case PRIMITIVE_TYPE::PRIMITIVE_CUBE:
		m_Primitives[index] = PrimitiveRenderData({ type, new Cube(desc, material) });
		break;
	case PRIMITIVE_TYPE::PRIMITIVE_PYRAMID:
		m_Primitives[index] = PrimitiveRenderData({ type, new Pyramid(desc, material) });
		break;
	case PRIMITIVE_TYPE::PRIMITIVE_SQUARE:
		m_Primitives[index] = PrimitiveRenderData({ type, new Square(desc, material) });
		break;
	case PRIMITIVE_TYPE::PRIMITIVE_TRIANGLE:
		m_Primitives[index] = PrimitiveRenderData({ type, new Triangle(desc, material) });
		break;
	}

	return std::make_shared<PrimitiveRenderData>(m_Primitives[index]);
}
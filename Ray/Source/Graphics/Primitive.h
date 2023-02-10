#pragma once
#include <vector>

struct RAY_API Vertex
{
	glm::vec3 m_Position;
	glm::vec2 m_TexCoord;
	glm::vec3 m_Normal;
};

struct RAY_API PrimitiveDesc
{
	glm::vec3 m_Col = glm::vec3(1.f);

	char m_Texture[MAX_PATH];
	char m_VertexShader[MAX_PATH];
	char m_FragmentShader[MAX_PATH];

	bool m_LightSource = false;

	glm::vec3 m_Pos = glm::vec3(0.f);
	glm::vec3 m_Rotation = glm::vec3(0.f);

	glm::vec3 m_RotationVel = glm::vec3(0.f);
	glm::vec3 m_Velocity = glm::vec3(0.f);
};

struct RAY_API InternalPrimitiveData
{
	glm::vec3 m_TranslationScale;
	glm::vec3 m_RotationScale;
};

/* DONT USE THIS IF YOU DONT KNOW WHAT THIS IS */
static std::unordered_map<std::string, InternalPrimitiveData> m_InternalData;

class RAY_API Primitive
{
public:
	Primitive() = default;
	Primitive(const PrimitiveDesc& desc);
	~Primitive() {}

	virtual void Render(float dt);

	void UpdatePrimitive();

public:
	glm::vec3 GetTranslationScale();
	glm::vec3 GetRotationScale();

	void SetTranslationScale(glm::vec3 s);
	void SetRotationScale(glm::vec3 s);

	PrimitiveDesc* GetDescription();

public:
	PrimitiveDesc m_Description;
	std::string m_InternalName;
	PRIMITIVE_TYPE m_Type;

protected:
	int m_Index;

	InternalPrimitiveData m_Data;

	glm::mat4 m_Model;

	ShaderProgramPtr m_Shader;
	TexturePtr m_Texture;
	VertexArrayObjectPtr m_VAO;
	
	UniformBufferPtr m_UniformBuffer;
};

class RAY_API Triangle : public Primitive
{
public:
	Triangle() = default;
	Triangle(const PrimitiveDesc& desc);
	virtual void Render(float dt) override;

private:
};

class RAY_API Square : public Primitive
{
public:
	Square() = default;
	Square(const PrimitiveDesc& desc);
	virtual void Render(float dt) override;

private:
};

class RAY_API Pyramid : public Primitive
{
public:
	Pyramid() = default;
	Pyramid(const PrimitiveDesc& desc);
	virtual void Render(float dt) override;

private:
};

class RAY_API Cube : public Primitive
{
public:
	Cube() = default;
	Cube(const PrimitiveDesc& desc);
	virtual void Render(float dt) override;

private:
};

class RAY_API PrimitiveContainer
{
public:
	struct PrimitiveRenderData
	{
		PrimitiveDesc m_Desc;
		PRIMITIVE_TYPE m_Type;


		Cube c;
		Pyramid p;
		Square s;
		Triangle t;

		bool Initialized = false;
	};

public:
	static PrimitiveContainer& Instance()
	{
		static PrimitiveContainer instance;	   // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}
private:
	PrimitiveContainer() {}                    // Constructor? (the {} brackets) are needed here.

public:
	PrimitiveContainer(PrimitiveContainer const&) = delete;
	void operator=(PrimitiveContainer const&) = delete;

public:
	void Render(float dt);

	// add a primitive
	void Add(PRIMITIVE_TYPE type, const PrimitiveDesc& desc);

public:
	std::vector<PrimitiveRenderData> m_Primitives;
	std::vector<glm::vec3> m_LightPositions;
};
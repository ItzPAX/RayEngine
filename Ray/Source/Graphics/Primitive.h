#pragma once
#include <vector>
#include <unordered_map>

enum class PRIMITIVE_TYPE
{
	PRIMITIVE_CUBE = 0,
	PRIMITIVE_PYRAMID,
	PRIMITIVE_SQUARE,
	PRIMITIVE_TRIANGLE
};

struct RAY_API Vertex
{
	glm::vec3 m_Position;
	glm::vec2 m_TexCoord;
	glm::vec3 m_Normal;
};

struct RAY_API PrimitiveDesc
{
	char m_Texture[MAX_PATH];
	char m_VertexShader[MAX_PATH];
	char m_FragmentShader[MAX_PATH];

	glm::vec3 m_ObjectColor = glm::vec3(1.f);

	glm::vec3 m_Position = glm::vec3(0.f);
	glm::vec3 m_Rotation = glm::vec3(0.f);

	glm::vec3 m_Velocity = glm::vec3(0.f);
	glm::vec3 m_RotationVel = glm::vec3(0.f);
};

class RAY_API Primitive
{
public:
	Primitive() = default;
	Primitive(const PrimitiveDesc& desc, const MaterialDesc& material);
	~Primitive() {}

	virtual void Render(float dt);

	void UpdatePrimitive(float dt);
	void Delete();

public:
	glm::vec3 GetTranslationScale();
	glm::vec3 GetRotationScale();

	void SetTranslationScale(glm::vec3 s);
	void SetRotationScale(glm::vec3 s);

	PrimitiveDesc* GetDescription();
	MaterialDesc* GetMaterial();
	PRIMITIVE_TYPE GetType();

protected:
	MaterialDesc m_Material;
	PrimitiveDesc m_Description;
	PRIMITIVE_TYPE m_Type;

protected:
	int m_Index;

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
	Triangle(const PrimitiveDesc& desc, const MaterialDesc& material);
	virtual void Render(float dt) override;

private:
};

class RAY_API Square : public Primitive
{
public:
	Square() = default;
	Square(const PrimitiveDesc& desc, const MaterialDesc& material);
	virtual void Render(float dt) override;

private:
};

class RAY_API Pyramid : public Primitive
{
public:
	Pyramid() = default;
	Pyramid(const PrimitiveDesc& desc, const MaterialDesc& material);
	virtual void Render(float dt) override;

private:
};

class RAY_API Cube : public Primitive
{
public:
	Cube() = default;
	Cube(const PrimitiveDesc& desc, const MaterialDesc& material);
	virtual void Render(float dt) override;

private:
};

struct PrimitiveRenderData
{
	PRIMITIVE_TYPE m_Type;
	void* m_Data;

	template<typename T>
	T get_primitive()
	{
		return (T)m_Data;
	}
};

class RAY_API PrimitiveContainer
{
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

	std::shared_ptr<PrimitiveRenderData> Add(PRIMITIVE_TYPE type, const PrimitiveDesc& desc, const MaterialDesc& material);

public:
	std::unordered_map<int, PrimitiveRenderData> m_Primitives;
};
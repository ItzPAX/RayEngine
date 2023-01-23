#pragma once
#include <vector>

struct RAY_API Vertex
{
	glm::vec3 m_Position;
	glm::vec2 m_TexCoord;
	glm::vec3 m_Color;
};

struct RAY_API UniformData
{
	glm::mat4 u_ModelViewProj;
};

struct RAY_API PrimitiveDesc
{
	glm::vec3 m_Col = glm::vec3(1.f);

	char m_Texture[MAX_PATH];
	char m_VertexShader[MAX_PATH];
	char m_FragmentShader[MAX_PATH];

	// on init
	glm::vec3 m_Pos = glm::vec3(0.f);
	glm::vec3 m_RotationInit = glm::vec3(0.f);

	// on update
	glm::vec3 m_RotationUpdate = glm::vec3(0.f);
};

class RAY_API Primitive
{
public:
	Primitive(const PrimitiveDesc& desc);
	~Primitive();

	virtual void Render(glm::mat4 viewproj, float dt);

protected:
	PrimitiveDesc m_Description;

	glm::mat4 m_Model;

	ShaderProgramPtr m_Shader;
	TexturePtr m_Texture;
	VertexArrayObjectPtr m_VAO;
	UniformBufferPtr m_UniformBuffer;
};

class RAY_API Triangle : public Primitive
{
public:
	Triangle(const PrimitiveDesc& desc);
	virtual void Render(glm::mat4 viewproj, float dt) override;

private:
};

class RAY_API Square : public Primitive
{
public:
	Square(const PrimitiveDesc& desc);
	virtual void Render(glm::mat4 viewproj, float dt) override;

private:
};

class RAY_API Pyramid : public Primitive
{
public:
	Pyramid(const PrimitiveDesc& desc);
	virtual void Render(glm::mat4 viewproj, float dt) override;

private:
};

class RAY_API Cube : public Primitive
{
public:
	Cube(const PrimitiveDesc& desc);
	virtual void Render(glm::mat4 viewproj, float dt) override;

private:
};

class RAY_API PrimitiveContainer
{
public:
	static PrimitiveContainer& Instance()
	{
		static PrimitiveContainer instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}
private:
	PrimitiveContainer() {}                    // Constructor? (the {} brackets) are needed here.

public:
	PrimitiveContainer(PrimitiveContainer const&) = delete;
	void operator=(PrimitiveContainer const&) = delete;

public:
	void Render(glm::mat4 viewproj, float dt);

	// add a primitive
	void Add(Pyramid p) { m_Pyramids.push_back(p); }
	void Add(Cube p) { m_Cubes.push_back(p); }
	void Add(Square p) { m_Squares.push_back(p); }
	void Add(Triangle p) { m_Triangles.push_back(p); }

private:
	std::vector<Pyramid> m_Pyramids;
	std::vector<Cube> m_Cubes;
	std::vector<Square> m_Squares;
	std::vector<Triangle> m_Triangles;
};
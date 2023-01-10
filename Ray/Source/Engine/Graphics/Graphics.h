#pragma once

enum TriangleType
{
	TRIANGLE_LIST = 0,
	TRIANGLE_STRIP
};

class RAY_API Graphics {

	//Getters and Setters for singleton static class
private:
	static Graphics* inst;
public:
	static Graphics* Instance() { return inst; }

	//Constructor
public:
	Graphics();
	~Graphics();

public:
	static VertexArrayObjectPtr CreateVertexArrayObject(const VertexBufferDesc& data);
	static ShaderProgramPtr CreateShaderProgram(const ShaderProgramDesc& desc);
	static UniformBufferPtr CreateUniformBuffer(const UniformBufferDesc& desc);

public:
	static void Clear(Vec4D col);
	static void SetViewport(Rect size);

public:
	static void SetVertexArrayObject(const VertexArrayObjectPtr& vao);
	static void SetShaderProgram(const ShaderProgramPtr& program);
	static void SetUniformBuffer(const UniformBufferPtr& buffer, UINT32 slot);

public:
	static void DrawTriangles(const TriangleType& type, UINT32 vertexCount, UINT32 offset);
};
#pragma once

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

public:
	static void Clear(Vec4D col);
	static void SetViewport(Rect size);
	static void SetVertexArrayObject(const VertexArrayObjectPtr& vao);
	static void SetShaderProgram(const ShaderProgramPtr& program);

public:
	static void DrawTriangles(UINT32 vertexCount, UINT32 offset);
};
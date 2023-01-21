#pragma once

enum class TriangleType
{
	TRIANGLE_LIST = 0,
	TRIANGLE_STRIP
};

enum class CullType
{
	BACK_FACE = 0,
	FRONT_FACE,
	BOTH
};

enum class WindingOrder
{
	CLOCKWISE = 0,
	ANTI_CLOCKWISE
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
	static VertexArrayObjectPtr CreateVertexArrayObject(const VertexBufferDesc& vbDesc);
	static VertexArrayObjectPtr CreateVertexArrayObject(const VertexBufferDesc& vbDesc, const IndexBufferDesc& ibDesc);
	static ShaderProgramPtr CreateShaderProgram(const ShaderProgramDesc& desc);
	static UniformBufferPtr CreateUniformBuffer(const UniformBufferDesc& desc);
	static FPSCameraPtr CreateFPSCamera(const CameraDesc& cdesc, const FPSCameraDesc& fpsdesc);
	static FloatingCameraPtr CreateFloatingCamera(const CameraDesc& cdesc, const FPSCameraDesc& fpsdesc); // fps + flying
	static FrameBufferPtr CreateFrameBuffer();

public:
	static void SetViewport(glm::vec4 size);
	static void SetVertexArrayObject(const VertexArrayObjectPtr& vao);
	static void SetShaderProgram(const ShaderProgramPtr& program);
	static void SetUniformBuffer(const UniformBufferPtr& buffer, UINT32 slot);
	static void SetFaceCulling(const CullType& type);
	static void SetWindingOrder(const WindingOrder& order);

public:
	static void Clear(glm::vec4 col);
	static void Clear(glm::vec4 col, bool enableGLDepth, bool clearImGuiFrame);
	static void DrawTriangles(const TriangleType& type, UINT32 vertexCount, UINT32 offset);
	static void DrawIndexedTriangles(const TriangleType& type, UINT32 indicesCount);
};
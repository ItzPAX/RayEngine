#include "Ray.h"

Graphics* Graphics::inst;

Graphics::Graphics()
{
	inst = this;

	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = L"OGL3DDummyWindow";
	wc.lpfnWndProc = DefWindowProc;
	wc.style = CS_OWNDC;

	auto classId = RegisterClassEx(&wc);
	assert(classId);

	auto dummyWindow = CreateWindowEx(NULL, MAKEINTATOM(classId), L"", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, NULL, NULL);

	assert(dummyWindow);

	auto dummyDC = GetDC(dummyWindow);

	PIXELFORMATDESCRIPTOR dsc = {};
	dsc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	dsc.nVersion = 1;
	dsc.iPixelType = PFD_TYPE_RGBA;
	dsc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	dsc.cColorBits = 32;
	dsc.cAlphaBits = 8;
	dsc.cStencilBits = 8;
	dsc.iLayerType = PFD_MAIN_PLANE;

	auto format = ChoosePixelFormat(dummyDC, &dsc);
	SetPixelFormat(dummyDC, format, &dsc);

	auto dummyContext = wglCreateContext(dummyDC);
	assert(dummyContext);

	wglMakeCurrent(dummyDC, dummyContext);

	if (!gladLoadWGL(dummyDC))
	{
		Logger::PrintOGL3DError(L"Graphics->gladLoadWGL failed");
	}

	if (!gladLoadGL())
	{
		Logger::PrintOGL3DError(L"Graphics->gladLoadGL failed");
	}

	wglMakeCurrent(dummyDC, NULL);
	wglDeleteContext(dummyContext);
	ReleaseDC(dummyWindow, dummyDC);
	DestroyWindow(dummyWindow);
}

Graphics::~Graphics()
{
}

VertexArrayObjectPtr Graphics::CreateVertexArrayObject(const VertexBufferDesc& vbDesc)
{
	return std::make_shared<VertexArrayObject>(vbDesc);
}

VertexArrayObjectPtr Graphics::CreateVertexArrayObject(const VertexBufferDesc& vbDesc, const IndexBufferDesc& ibDesc)
{
	return std::make_shared<VertexArrayObject>(vbDesc, ibDesc);
}

ShaderProgramPtr Graphics::CreateShaderProgram(const ShaderProgramDesc& desc)
{
	return std::make_shared<ShaderProgram>(desc);
}

UniformBufferPtr Graphics::CreateUniformBuffer(const UniformBufferDesc& desc)
{
	return std::make_shared<UniformBuffer>(desc);
}

FrameBufferPtr Graphics::CreateFrameBuffer()
{
	return std::make_shared<FrameBuffer>();
}

void Graphics::Clear(Vec4D col)
{
	glClearColor(col.x(), col.y(), col.z(), col.w());
	glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::SetViewport(Rect size)
{
	glViewport(size.left(), size.top(), size.width(), size.height());
}

void Graphics::SetVertexArrayObject(const VertexArrayObjectPtr& vao)
{
	glBindVertexArray(vao->GetVAOId());
}

void Graphics::SetShaderProgram(const ShaderProgramPtr& program)
{
	glUseProgram(program->GetProgramId());
}

void Graphics::SetUniformBuffer(const UniformBufferPtr& buffer, UINT32 slot)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, buffer->GetBufferId());
}

void Graphics::SetFaceCulling(const CullType& type)
{
	auto cullType = GL_BACK;

	if (type == CullType::FRONT_FACE) cullType = GL_FRONT;
	else if (type == CullType::BACK_FACE) cullType = GL_BACK;
	else if (type == CullType::BOTH) cullType = GL_FRONT_AND_BACK;

	glEnable(GL_CULL_FACE);
	glCullFace(cullType);
}

void Graphics::SetWindingOrder(const WindingOrder& order)
{
	auto orderType = GL_CW;

	if (order == WindingOrder::CLOCKWISE) orderType = GL_CW;
	else if (order == WindingOrder::ANTI_CLOCKWISE) orderType = GL_CCW;

	glFrontFace(orderType);
}

void Graphics::DrawTriangles(const TriangleType& type, UINT32 vertexCount, UINT32 offset)
{
	auto glTriType = GL_TRIANGLES;

	switch (type)
	{
	case TriangleType::TRIANGLE_LIST: { glTriType = GL_TRIANGLES; break; }
	case TriangleType::TRIANGLE_STRIP: { glTriType = GL_TRIANGLE_STRIP; break; }

	default:
		break;
	}

	glDrawArrays(glTriType, offset, vertexCount);
}

void Graphics::DrawIndexedTriangles(const TriangleType& type, UINT32 indicesCount)
{
	auto glTriType = GL_TRIANGLES;

	switch (type)
	{
	case TriangleType::TRIANGLE_LIST: { glTriType = GL_TRIANGLES; break; }
	case TriangleType::TRIANGLE_STRIP: { glTriType = GL_TRIANGLE_STRIP; break; }

	default:
		break;
	}

	glDrawElements(glTriType, indicesCount, GL_UNSIGNED_INT, nullptr);
}

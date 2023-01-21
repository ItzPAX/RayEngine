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

FPSCameraPtr Graphics::CreateFPSCamera(const CameraDesc& cdesc, const FPSCameraDesc& fpsdesc)
{
	return std::make_shared<FPSCamera>(cdesc, fpsdesc);
}

// uses the same params as the fps camera, cuz it is basically a fps camera with added flying
FloatingCameraPtr Graphics::CreateFloatingCamera(const CameraDesc& cdesc, const FPSCameraDesc& fpsdesc)
{
	return std::make_shared<FloatingCamera>(cdesc, fpsdesc);
}

FrameBufferPtr Graphics::CreateFrameBuffer()
{
	return std::make_shared<FrameBuffer>();
}

void Graphics::Clear(glm::vec4 col)
{
	glClearColor(col.x, col.y, col.z, col.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::Clear(glm::vec4 col, bool enableGLDepth, bool clearImGuiFrame)
{
	glClearColor(col.x, col.y, col.z, col.w);
	glClear(GL_COLOR_BUFFER_BIT);

	if (clearImGuiFrame)
	{
		ImGui_ImplOpenGL3_NewFrame();
	}
	if (enableGLDepth)
	{
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
}

void Graphics::SetViewport(glm::vec4 size)
{
	glViewport(size.x, size.y, size.z, size.w);
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

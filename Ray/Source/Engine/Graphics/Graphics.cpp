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

VertexArrayObjectPtr Graphics::CreateVertexArrayObject(const VertexBufferDesc& data)
{
	return std::make_shared<VertexArrayObject>(data);
}

ShaderProgramPtr Graphics::CreateShaderProgram(const ShaderProgramDesc& desc)
{
	return std::make_shared<ShaderProgram>(desc);
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

void Graphics::DrawTriangles(UINT32 vertexCount, UINT32 offset)
{
	glDrawArrays(GL_TRIANGLES, offset, vertexCount);
}

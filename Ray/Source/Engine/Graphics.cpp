#include "Ray.h"
#include <assert.h>
#include <stdexcept>

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
		Logger::PrintLog(L"OGraphics - Error - gladLoadWGL failed");
		throw std::runtime_error("OGraphics - Error - gladLoadWGL failed");
	}

	if (!gladLoadGL())
	{
		Logger::PrintLog(L"OGraphics - Error - gladLoadGL failed");
		throw std::runtime_error("OGraphics - Error - gladLoadGL failed");
	}

	wglMakeCurrent(dummyDC, NULL);
	wglDeleteContext(dummyContext);
	ReleaseDC(dummyWindow, dummyDC);
	DestroyWindow(dummyWindow);
}

Graphics::~Graphics()
{
}

void Graphics::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);
}
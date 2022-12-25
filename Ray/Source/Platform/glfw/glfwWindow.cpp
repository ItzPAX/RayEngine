#include "Ray.h"
#include "glfwWindow.h"

#define DCX_USESTYLE 0x00010000

namespace glfw {

	Window::Window(STRING title, HICON icon, WindowType type)
		: glfw::SubObject(title, icon), m_Type(type)
	{
		Size(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	}

	Window::~Window()
	{
	}
	VOID Window::Initialize()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		m_Handle = glfwCreateWindow(Size().cx, Size().cy, m_Title.c_str(), NULL, NULL);
		glfwMakeContextCurrent(Handle());

		// init glad before calling any opengl funcs
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		glViewport(0, 0, Size().cx, Size().cy);

		// set callbacks to customize window
		glfwSetFramebufferSizeCallback(Handle(), FramebufferSizeCallback);

		Update();
	}

	VOID Window::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}
}
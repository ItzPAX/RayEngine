#include "Ray.h"
#include "Simulation.h"
#include "Engine/Splashscreen.h"

namespace Ray {

	Simulation::Simulation()
		: Win32::Window(L"MainApplication", NULL)
	{

	}

	Simulation::~Simulation()
	{
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	VOID Simulation::PreInitialize()
	{
		// log info about game
		Logger::PrintDebugSeperator();
		Logger::PrintLog(L"Application Starting...\n");
		Logger::PrintLog(L"Game Name: %s\n", PerGameSettings::GameName());
		Logger::PrintLog(L"Boot Time: %s\n", Time::GetDateTimeString().c_str());
		Logger::PrintLog(L"Engine Mode: %s\n", Engine::EngineModeToString().c_str());
		Logger::PrintDebugSeperator();

		// start our splashscreen
		SplashScreen::Open();

		Win32::Window::RegisterNewClass();
		Win32::Window::Initialize();

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
		glfwMakeContextCurrent(window);

		// init glad before calling any opengl funcs
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		}

		glViewport(0, 0, 800, 600);

		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		while (!glfwWindowShouldClose(window))
		{
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		glfwTerminate();
	}

	LRESULT Simulation::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{

		}
		return Window::MessageHandler(hwnd, message, wParam, lParam);
	}
}
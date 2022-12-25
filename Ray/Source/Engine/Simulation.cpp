#include "Ray.h"
#include "Simulation.h"
#include "Engine/Splashscreen.h"

namespace Ray {

	Simulation::Simulation()
		: glfw::Window("MainApplication", NULL),
		  Win32::Window(L"MainApplication", NULL)
	{

	}

	Simulation::~Simulation()
	{
	}

	VOID Simulation::w32PreInitialize()
	{
		// log info about game
		Logger::PrintDebugSeperator();
		Logger::PrintLog(L"Application Starting...\n");
		Logger::PrintLog(L"Game Name: %s\n", PerGameSettings::GameName());
		Logger::PrintLog(L"Boot Time: %s\n", Time::GetDateTimeString().c_str());
		Logger::PrintLog(L"Engine Mode: %s\n", Engine::EngineModeToString().c_str());
		Logger::PrintLog(L"Starting in DirectX Mode...\n");
		Logger::PrintDebugSeperator();

		// start our splashscreen
		SplashScreen::Open();

		Win32::Window::RegisterNewClass();
		Win32::Window::Initialize();
	}
	LRESULT Simulation::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{

		}
		return Win32::Window::MessageHandler(hwnd, message, wParam, lParam);
	}


	GLFWwindow* Simulation::glfwPreInitialize()
	{
		// log info about game
		Logger::PrintDebugSeperator();
		Logger::PrintLog(L"Application Starting...\n");
		Logger::PrintLog(L"Game Name: %s\n", PerGameSettings::GameName());
		Logger::PrintLog(L"Boot Time: %s\n", Time::GetDateTimeString().c_str());
		Logger::PrintLog(L"Engine Mode: %s\n", Engine::EngineModeToString().c_str());
		Logger::PrintLog(L"Starting in OpenGL Mode...\n");
		Logger::PrintDebugSeperator();

		// start our splashscreen
		SplashScreen::Open();

		glfw::Window::Initialize();

		return glfw::Window::Handle();
	}
}
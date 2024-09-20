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

	VOID Simulation::PreInitialize()
	{
		// log info about game
		Logger::PrintDebugSeperator();
		Logger::PrintLog(L"Application Starting...\n");
		Logger::PrintLog(L"Game Name: %s\n", PerGameSettings::GameName());
		Logger::PrintLog(L"Boot Time: %s\n", Time::GetDateTimeString().c_str());
		Logger::PrintLog(L"Engine Mode: %s\n", Engine::EngineModeToString().c_str());
		Logger::PrintLog(L"OpenGL Version: 4.6\n");
		Logger::PrintDebugSeperator();

		// start our splashscreen
		SplashScreen::Open();

		// create window
		Win32::Window::RegisterNewClass();
		Win32::Window::Initialize();

		// make window opengl ready
		Win32::Window::MakeCurrentContext();

		// set our viewport
		Graphics::Instance()->SetViewport(GetInnerSize());

		Graphics::Instance()->CreateScene("MainScene");
		Graphics::Instance()->CreateScene("MainScene1");
		SceneContainer::Instance().SetActiveScene("MainScene");

		// register mouse device for accurate input
		RAWINPUTDEVICE rid;
		rid.usUsagePage = 0x0001;
		rid.usUsage = 0x0002;
		rid.dwFlags = 0;
		rid.hwndTarget = Handle();
		if (!RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE)))
			Logger::PrintLog(L"Failed to register raw input device");
	}

	LRESULT Simulation::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		default:
			break;
		}
		return Win32::Window::MessageHandler(hwnd, message, wParam, lParam);
	}
}
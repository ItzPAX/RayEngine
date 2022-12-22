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
		return Window::MessageHandler(hwnd, message, wParam, lParam);
	}
}
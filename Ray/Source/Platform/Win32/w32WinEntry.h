#include "Ray.h"

#include "w32IApplication.h"
#include "Common/CmdLineArgs.h"
#include "Engine/Splashscreen.h"

extern Win32::IApplication* EntryApplication();

int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	auto EntryApp = EntryApplication();

	// fill game settings
	PerGameSettings GameSettings;
	EntryApp->SetupPerGameSettings();

	// check for command line args
	CmdLineArgs::ReadArguments();

	// start logger
	Logger logger;

	// start graphics
	Graphics graphics;

	// tell engine to initialize
	EntryApp->PreInitialize();
	
	// tell game to start initializing
	EntryApp->Initialize();

	std::chrono::system_clock::time_point m_PreviousTime;

	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		// If there are Window messages process them
		if (PeekMessage(&msg, HWND(), NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// compute delta time
			auto currentTime = std::chrono::system_clock::now();
			auto elapsedSeconds = std::chrono::duration<double>();
			if (m_PreviousTime.time_since_epoch().count())
				elapsedSeconds = currentTime - m_PreviousTime;

			m_PreviousTime = currentTime;
			auto deltaTime = elapsedSeconds.count();

			// Let the game update
			EntryApp->Update(deltaTime);
		}
	}

	// let the game know we quit
	EntryApp->Quit();

	return 0;
}
#include "Ray.h"

#include "glfwIApplication.h"
#include "Common/CmdLineArgs.h"
#include "Engine/Splashscreen.h"

extern glfw::IApplication* glfwEntryApplication();

#ifdef OpenGL
int CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	auto EntryApp = glfwEntryApplication();

	// fill game settings
	PerGameSettings GameSettings;
	EntryApp->SetupPerGameSettings();

	// check for command line args
	CmdLineArgs::ReadArguments();

	// start logger
	Logger logger;

	// tell engine to initialize
	GLFWwindow* simwin = EntryApp->glfwPreInitialize();

	// tell game to start initializing
	EntryApp->glfwInitialize(simwin);

	while (!glfwWindowShouldClose(simwin))
	{
		// focus on our main window before updating
		glfwMakeContextCurrent(simwin);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// let game update
		EntryApp->glfwUpdate(simwin);
	
		// Swap buffer
		glfwSwapBuffers(simwin);

		// queue events
		glfwPollEvents();
	}
	glfwTerminate();

	return 0;
}
#endif
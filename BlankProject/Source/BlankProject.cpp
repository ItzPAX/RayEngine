#include "BlankProject.h"
#include "Engine/Simulation.h"

// WindowsAPI
#include "Platform/Win32/w32WinEntry.h"

// GLFW (OpenGL)
#include "Platform/glfw/glfwWinEntry.h"

class BlankProject : public Ray::Simulation {

	// Application

public:
	/* Application Constuctor */
	BlankProject() {};

	/* Application Deconstructor */
	~BlankProject() {};


public:
	/* Called to setup our per game settings  */
	VOID SetupPerGameSettings();

	// WINAPI Init
	VOID w32Initialize() {};
	// WINAPI Update
	VOID w32Update() {};

	// GLFW Init
	VOID glfwInitialize(GLFWwindow* window) {};
	// GLFW Update
	VOID glfwUpdate(GLFWwindow* window) {};
};

ENTRYAPPGLFW(BlankProject)

VOID BlankProject::SetupPerGameSettings()
{
	/* Set the Per Game Settings */
	PerGameSettings::SetGameName(IDS_PERGAMENAME);
	PerGameSettings::SetShortName(IDS_SHORTNAME);
	PerGameSettings::SetMainIcon(IDI_MAINICON);
	PerGameSettings::SetGameSplashURL(IDS_SPLASHURL);
}
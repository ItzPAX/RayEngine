#include "BlankProject.h"
#include "Engine/Simulation.h"

// WindowsAPI
#include "Platform/Win32/w32WinEntry.h"

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

	// How the engine updates each frame
	VOID EngineUpdate(float deltatime)
	{
		Graphics::Instance()->Clear(glm::vec4(0.f, 0.f, 0.f, 1.f), true, false);

		FloatingCamera::GetFloatingCam().Think(MouseSpeed(), deltatime);

		SceneContainer::Instance().GetActiveScene()->Render(deltatime);

		Update(deltatime, SceneContainer::Instance().GetActiveScene()->GetSceneElapsedTime());

		Simulation::Present(true);
	}

	// Initialize
	VOID Initialize()
	{
		Graphics::Instance()->SetBlending(true);
	}

	// Update
	VOID Update(float deltatime, float elapsedtime)
	{
		SceneContainer::Instance().GetActiveScene()->GetLightingManager()->m_SpotLight.m_Position = FloatingCamera::GetFloatingCam().Position();
		SceneContainer::Instance().GetActiveScene()->GetLightingManager()->m_SpotLight.m_Direction = FloatingCamera::GetFloatingCam().Front();
	}

	// Quit
	VOID Quit() 
	{
	}
};

ENTRYAPP(BlankProject)

VOID BlankProject::SetupPerGameSettings()
{
	/* Set the Per Game Settings */
	PerGameSettings::SetGameName(IDS_PERGAMENAME);
	PerGameSettings::SetShortName(IDS_SHORTNAME);
	PerGameSettings::SetMainIcon(IDI_MAINICON);
	PerGameSettings::SetGameSplashURL(IDS_SPLASHURL);
}
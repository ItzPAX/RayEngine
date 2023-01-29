#include "BlankProject.h"
#include "Engine/Simulation.h"

// WindowsAPI
#include "Platform/Win32/w32WinEntry.h"

class BlankProject : public Ray::Simulation, public Data {

	// Application

public:
	/* Application Constuctor */
	BlankProject() {};

	/* Application Deconstructor */
	~BlankProject() {};


public:
	/* Called to setup our per game settings  */
	VOID SetupPerGameSettings();

	// Init
	VOID Initialize() 
	{
		// init a framebuffer
		m_FrameBuffer = Graphics::Instance()->CreateFrameBuffer();
	}

	// Update
	VOID Update(float deltatime)
	{
		Graphics::Instance()->Clear(glm::vec4(0.26f, 0.26f, 0.26f, 0.5f), true, false);

		UI::Instance().RenderUI(m_FrameBuffer->Textures(), { FloatingCamera::GetFloatingCam().Position(), FloatingCamera::GetFloatingCam().Pitch(), FloatingCamera::GetFloatingCam().Yaw()});
		FloatingCamera::GetFloatingCam().Think(MouseSpeed(), deltatime);

		m_FrameBuffer->Bind();
		Graphics::Instance()->Clear(glm::vec4(0.f, 0.f, 0.f, 1.f), true, false);

		PrimitiveContainer::Instance().Render(FloatingCamera::GetFloatingCam().GetViewProj(), deltatime);

		m_FrameBuffer->Unbind();

		Simulation::Present(false);
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
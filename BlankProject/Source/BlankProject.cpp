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
		Graphics::Instance()->Clear(glm::vec4(0.26f, 0.26f, 0.26f, 0.5f), true, false);
		FloatingCamera::GetFloatingCam().Think(MouseSpeed(), deltatime);

		if (Engine::GetMode() == EDITOR)
		{
			UI::Instance().RenderUI(m_FrameBuffer->Textures(), UI::CameraType::CAMERA_FLOATING);
			Simulation::m_FrameBuffer->Bind();

			Graphics::Instance()->Clear(glm::vec4(0.f, 0.f, 0.f, 1.f), true, false);
		}

		Update(deltatime);

		PrimitiveContainer::Instance().Render(deltatime);

		if (Engine::GetMode() == EDITOR)
		{
			Simulation::m_FrameBuffer->Unbind();
		}

		Simulation::Present(false);
	}

	// Init
	VOID Initialize()
	{
		Graphics::Instance()->CreateCube(
			{
				glm::vec3(1.f),
				"",
				"D:/MyProgramming/OpenGL/RayEngine/Build/Debug/Content/Engine/Shaders/BasicShader.vert",
				"D:/MyProgramming/OpenGL/RayEngine/Build/Debug/Content/Engine/Shaders/BasicShader.frag",
				glm::vec3(1,1,1)
			}
		);

		Graphics::Instance()->CreateCube(
			{
				glm::vec3(1.f),
				"",
				"D:/MyProgramming/OpenGL/RayEngine/Build/Debug/Content/Engine/Shaders/BasicShader.vert",
				"D:/MyProgramming/OpenGL/RayEngine/Build/Debug/Content/Engine/Shaders/BasicShaderLight.frag",
			}
		);
	}

	// Update
	VOID Update(float deltatime)
	{

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
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
		// Init ImGui
		m_UI = std::make_shared<UI>(Handle());

		m_Cube[0] = Graphics::Instance()->CreateCube
		(
			{
				glm::vec3(0,0,0),
				glm::vec3(1.f,1.f,1.f),
				"D:/nerd.jpg",
				L"D:/MyProgramming/OpenGL/RayEngine/Build/Debug/Content/Engine/Shaders/ExampleShader.vert",
				L"D:/MyProgramming/OpenGL/RayEngine/Build/Debug/Content/Engine/Shaders/ExampleShader.frag"
			}
		);

		m_Cube[1] = Graphics::Instance()->CreateCube
		(
			{
				glm::vec3(2,0,0),
				glm::vec3(1.f,1.f,1.f),
				"D:/container.jpg",
				L"D:/MyProgramming/OpenGL/RayEngine/Build/Debug/Content/Engine/Shaders/ExampleShader.vert",
				L"D:/MyProgramming/OpenGL/RayEngine/Build/Debug/Content/Engine/Shaders/ExampleShader.frag"
			}
		);

		m_Pyramid = Graphics::Instance()->CreatePyramid
		(
			{
				glm::vec3(-2,0,0),
				glm::vec3(1.f,1.f,1.f),
				"D:/wall.jpg",
				L"D:/MyProgramming/OpenGL/RayEngine/Build/Debug/Content/Engine/Shaders/ExampleShader.vert",
				L"D:/MyProgramming/OpenGL/RayEngine/Build/Debug/Content/Engine/Shaders/ExampleShader.frag"
			}
		);

		// init a framebuffer
		m_FrameBuffer = Graphics::Instance()->CreateFrameBuffer();

		m_FloatingCamera = Graphics::Instance()->CreateFloatingCamera
		(
			{
				90.f,							// FOV
				3.f,							// SPEED
				0.1f,							// NEAR
				1000.f,							// FAR
				glm::vec3(0.f, 2.f, 8.f)		// START POS
			},
			{
				0.2f							// MOUSE SENS
			}
		);

		
	}

	// Update
	VOID Update(float deltatime)
	{
		Graphics::Instance()->Clear(glm::vec4(0.26f, 0.26f, 0.26f, 0.5f), true, false);

		m_UI->RenderUI(m_FrameBuffer->Textures(), { m_FloatingCamera->Position(), m_FloatingCamera->Pitch(), m_FloatingCamera->Yaw() });
		m_FloatingCamera->Think(MouseSpeed(), m_UI->SceneRect(), m_UI->SceneActive(), Handle(), deltatime);

		m_FrameBuffer->Bind();
		Graphics::Instance()->Clear(glm::vec4(0.f, 0.f, 0.f, 1.f), true, false);

		m_Cube[0]->Render(m_FloatingCamera->GetViewProj());
		m_Cube[1]->Render(m_FloatingCamera->GetViewProj());
		m_Pyramid->Render(m_FloatingCamera->GetViewProj());

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
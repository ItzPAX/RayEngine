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

		UI::Instance().RenderUI(m_FrameBuffer->Textures(), UI::CameraType::CAMERA_FLOATING);
		Simulation::m_FrameBuffer->Bind();

		Graphics::Instance()->Clear(glm::vec4(0.f, 0.f, 0.f, 1.f), true, false);

		static float elapsed = 0;
		elapsed += deltatime;

		Graphics::Instance()->m_DrawCallsThisFrame = 0;
		Graphics::Instance()->m_TrianglesThisFrame = 0;

		Update(deltatime, elapsed);

		PrimitiveContainer::Instance().Render(deltatime);
		ModelContainer::Instance().Render(deltatime);

		Simulation::m_FrameBuffer->Unbind();

		Simulation::Present(false);
	}

	// Init
	PrimitivePtr prim[10];
	ModelPtr model;

	VOID Initialize()
	{
		LightingManager::Instance().m_SpotLight = SpotLight{
			glm::vec3(1.f, 1.f, 1.f),			// position
			glm::vec3(1.f, 1.f, 1.f),			// direction
			glm::vec3(0.2f, 0.2f, 0.2f),		// ambient
			glm::vec3(0.5f, 0.5f, 0.5f),		// diffuse
			glm::vec3(1.0f, 1.0f, 1.0f),		// specular
			glm::cos(glm::radians(17.5f)),		// outer cutoff
			glm::cos(glm::radians(12.5f)),		// inner cutoff
			1.f,								// constant
			0.09f,								// linear
			0.032f								// quadratic
		};
		LightingManager::Instance().m_DirectionalLight = DirectionalLight{
			glm::vec3(-0.2f, -1.0f, -0.3f),		// direction
			glm::vec3(0.2f, 0.2f, 0.2f),		// ambient
			glm::vec3(0.5f, 0.5f, 0.5f),		// diffuse
			glm::vec3(1.0f, 1.0f, 1.0f),		// specular
		};
		Graphics::Instance()->SetBlending(true);
	}

	// Update
	VOID Update(float deltatime, float elapsedtime)
	{
		LightingManager::Instance().m_SpotLight.m_Position = FloatingCamera::GetFloatingCam().Position();
		LightingManager::Instance().m_SpotLight.m_Direction = FloatingCamera::GetFloatingCam().Front();
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
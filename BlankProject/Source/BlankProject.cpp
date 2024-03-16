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

		if (Engine::GetMode() == EDITOR)
		{
			UI::Instance().RenderUI(m_FrameBuffer->Textures(), UI::CameraType::CAMERA_FLOATING);
			Simulation::m_FrameBuffer->Bind();

			Graphics::Instance()->Clear(glm::vec4(0.f, 0.f, 0.f, 1.f), true, false);
		}

		static float elapsed = 0;
		elapsed += deltatime;

		Update(deltatime, elapsed);

		PrimitiveContainer::Instance().Render(deltatime);

		if (Engine::GetMode() == EDITOR)
		{
			Simulation::m_FrameBuffer->Unbind();
		}

		Simulation::Present(false);
	}

	// Init
	PrimitivePtr prim[2];

	VOID Initialize()
	{
		prim[0] = Graphics::Instance()->CreatePrimitive(
			PRIMITIVE_TYPE::PRIMITIVE_CUBE,
			{
				"",																							
				"C:/Users/Deniz/Desktop/RayEngine/Build/Release/Content/Engine/Shaders/BasicShader.vert",
				"C:/Users/Deniz/Desktop/RayEngine/Build/Release/Content/Engine/Shaders/BasicShaderLight.frag",
				glm::vec3(1.f),
				glm::vec3(1.f)
			},
			Materials::debug
		);

		prim[1] = Graphics::Instance()->CreatePrimitive(
			PRIMITIVE_TYPE::PRIMITIVE_CUBE,
			{
				"",																							// texture
				"C:/Users/Deniz/Desktop/RayEngine/Build/Release/Content/Engine/Shaders/BasicShader.vert",	// vertex
				"C:/Users/Deniz/Desktop/RayEngine/Build/Release/Content/Engine/Shaders/BasicShader.frag",	// fragment
				glm::vec3(1.f),																				// color
				glm::vec3(1.f)																				// pos
			},
			Materials::debug
		);

		LightingManager::Instance().m_StudioLight = LightingDesc{
			glm::vec3(1.f),						// position
			glm::vec3(0.2f, 0.2f, 0.2f),		// ambient
			glm::vec3(0.5f, 0.5f, 0.5f),		// diffuse
			glm::vec3(1.0f, 1.0f, 1.0f),		// specular
		};
	}

	// Update
	VOID Update(float deltatime, float elapsedtime)
	{
		LightingManager::Instance().m_StudioLight.m_Position = glm::vec3(sin(elapsedtime) * 5, 0, 0);
		prim[1].get()->get_primitive<Cube*>()->GetDescription()->m_Position = glm::vec3(sin(elapsedtime) * 5, 0, 0);
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
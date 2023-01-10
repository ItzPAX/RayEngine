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
		float polygonVertices[] = {
			-0.5f, -0.5f, 0.f,		// POSITION
			1,0,0,					// COLOR

			-0.5f, 0.5f, 0.f,
			0,1,0,

			0.5f, -0.5f, 0.f,
			0,0,1,

			0.5f, 0.5f, 0.f,
			1,1,0
		};

		VertexAttribute attribList[] = {
			3,						// POSITION
			3						// COLOR
		};

		m_VAO = Graphics::Instance()->CreateVertexArrayObject
		({ 
			polygonVertices,
			sizeof(float) * (3 + 3),
			4,

			attribList,
			2
		});

		m_Uniform = Graphics::Instance()->CreateUniformBuffer
		({
			sizeof(UniformData)
		});

		m_Shader = Graphics::Instance()->CreateShaderProgram
		({
			L"D:/MyProgramming/OpenGL/RayEngine/Build/Debug/Content/Engine/Shaders/ExampleShader.vert", 
			L"D:/MyProgramming/OpenGL/RayEngine/Build/Debug/Content/Engine/Shaders/ExampleShader.frag"
		});

		m_Shader->SetUniformBufferSlot("UniformData", 0);
	}

	// Update
	VOID Update() 
	{
		// compute delta time
		auto currentTime = std::chrono::system_clock::now();
		auto elapsedSeconds = std::chrono::duration<double>();

		if (m_PreviousTime.time_since_epoch().count())
			elapsedSeconds = currentTime - m_PreviousTime;

		m_PreviousTime = currentTime;

		auto deltaTime = (float)elapsedSeconds.count();

		m_Scale += 6.28 * deltaTime;
		auto currentScale = sin(m_Scale);

		UniformData data = { currentScale };
		m_Uniform->SetData(&data);

		Graphics::Instance()->Clear(Vec4D(0.3f,0.3f,0.7f,0.5f));

		Graphics::Instance()->SetVertexArrayObject(m_VAO);
		Graphics::Instance()->SetUniformBuffer(m_Uniform, 0);
		Graphics::Instance()->SetShaderProgram(m_Shader);

		Graphics::Instance()->DrawTriangles(TRIANGLE_STRIP, m_VAO->GetVertexBufferSize(), 0);

		Simulation::Present(false);
	}

	// Quit
	VOID Quit() {}
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
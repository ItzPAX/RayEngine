#include "BlankProject.h"
#include "Engine/Simulation.h"

// WindowsAPI
#include "Platform/Win32/w32WinEntry.h"

VertexArrayObjectPtr m_VAO;
ShaderProgramPtr m_Shader;

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

	// Init
	VOID Initialize() 
	{
		float triangleVertices[] = {
			-0.5f, -0.5f, 0.f,		// POSITION
			1,0,0,					// COLOR

			0.5f, -0.5f, 0.f,
			0,1,0,

			0.f, 0.5f, 0.f,
			0,0,1
		};

		VertexAttribute attribList[] = {
			3,						// POSITION
			3						// COLOR
		};

		m_VAO = Graphics::Instance()->CreateVertexArrayObject
		({ 
			triangleVertices, 
			sizeof(float) * (3 + 3),
			3,

			attribList,
			2
		});

		m_Shader = Graphics::Instance()->CreateShaderProgram
		({
			L"D:/MyProgramming/OpenGL/RayEngine/Build/Debug/Content/Engine/Shaders/ExampleShader.vert", 
			L"D:/MyProgramming/OpenGL/RayEngine/Build/Debug/Content/Engine/Shaders/ExampleShader.frag"
		});
	}

	// Update
	VOID Update() 
	{
		Graphics::Instance()->Clear(Vec4D(0.3f,0.3f,0.7f,0.5f));
		Graphics::Instance()->SetShaderProgram(m_Shader);
		Graphics::Instance()->SetVertexArrayObject(m_VAO);

		Graphics::Instance()->DrawTriangles(m_VAO->GetVertexBufferSize(), 0);

		Simulation::Present(true);
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
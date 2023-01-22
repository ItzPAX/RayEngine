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

		m_Texture = Graphics::Instance()->CreateTexture("D:\\container.jpg");

		Vertex verticesList[] =
		{
			/*POS - TEXCOORD - COLOR*/
			{glm::vec3(0.5f, 0.5f, 0.f),   glm::vec2(1.f, 1.f),  glm::vec3(1.f,0.f,0.f)},
			{glm::vec3(0.5f, -0.5f, 0.f),  glm::vec2(1.f, 0.f),  glm::vec3(0.f,1.f,0.f)},
			{glm::vec3(-0.5f, -0.5f, 0.f), glm::vec2(0.f, 0.f),  glm::vec3(0.f,0.f,1.f)},
			{glm::vec3(-0.5f, 0.5f, 0.f),  glm::vec2(0.f, 1.f),  glm::vec3(1.f,1.f,0.f)}
		};

		UINT32 indices[] = 
		{
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		VertexAttribute attribList[] = 
		{
			sizeof(glm::vec3) / sizeof(float),		// POSITION
			sizeof(glm::vec2) / sizeof(float),		// TEXCOORD
			sizeof(glm::vec3) / sizeof(float)		// COLOR
		};

		m_VAO = Graphics::Instance()->CreateVertexArrayObject
		(
			{
				verticesList,
				sizeof(Vertex),
				sizeof(verticesList) / sizeof(Vertex),

				attribList,
				sizeof(attribList) / sizeof(VertexAttribute)
			},
			{
				indices,
				sizeof(indices)
			}
		);

		m_Shader = Graphics::Instance()->CreateShaderProgram
		({
			L"D:/MyProgramming/OpenGL/RayEngine/Build/Debug/Content/Engine/Shaders/ExampleShader.vert", 
			L"D:/MyProgramming/OpenGL/RayEngine/Build/Debug/Content/Engine/Shaders/ExampleShader.frag"
		});

		m_Uniform = Graphics::Instance()->CreateUniformBuffer
		({
			sizeof(UniformData)
		});

		m_Shader->SetUniformBufferSlot("EngineData", 0);

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

		glm::mat4 u_ModelViewProjection = m_FloatingCamera->GetViewProj();

		UniformData data = { u_ModelViewProjection };
		m_Uniform->SetData(&data);

		m_FrameBuffer->Bind();

		Graphics::Instance()->Clear(glm::vec4(0.f, 0.f, 0.f, 1.f), true, false);
		
		Graphics::Instance()->SetTexture(m_Texture);
		Graphics::Instance()->SetVertexArrayObject(m_VAO);
		Graphics::Instance()->SetUniformBuffer(m_Uniform, 0);
		Graphics::Instance()->SetShaderProgram(m_Shader);
		
		Graphics::Instance()->DrawIndexedTriangles(TriangleType::TRIANGLE_LIST, 6);

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
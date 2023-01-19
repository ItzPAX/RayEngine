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

		glm::vec3 positionsList[] =
		{
			//front face
			glm::vec3(-0.5f,-0.5f,-0.5f),
			glm::vec3(-0.5f,0.5f,-0.5f),
			glm::vec3(0.5f,0.5f,-0.5f),
			glm::vec3(0.5f,-0.5f,-0.5f),

			//back face
			glm::vec3(0.5f,-0.5f,0.5f),
			glm::vec3(0.5f,0.5f,0.5f),
			glm::vec3(-0.5f,0.5f,0.5f),
			glm::vec3(-0.5f,-0.5f,0.5f)
		};

		glm::vec2 texcoordsList[] =
		{
			glm::vec2(0,0),
			glm::vec2(0,1),
			glm::vec2(1,0),
			glm::vec2(1,1)
		};

		Vertex verticesList[] =
		{
			//front face
			{ positionsList[0],texcoordsList[1] },
			{ positionsList[1],texcoordsList[0] },
			{ positionsList[2],texcoordsList[2] },
			{ positionsList[3],texcoordsList[3] },

			//back face
			{ positionsList[4],texcoordsList[1] },
			{ positionsList[5],texcoordsList[0] },
			{ positionsList[6],texcoordsList[2] },
			{ positionsList[7],texcoordsList[3] },

			//top face
			{ positionsList[1],texcoordsList[1] },
			{ positionsList[6],texcoordsList[0] },
			{ positionsList[5],texcoordsList[2] },
			{ positionsList[2],texcoordsList[3] },

			//bottom face
			{ positionsList[7],texcoordsList[1] },
			{ positionsList[0],texcoordsList[0] },
			{ positionsList[3],texcoordsList[2] },
			{ positionsList[4],texcoordsList[3] },

			//right face
			{ positionsList[3],texcoordsList[1] },
			{ positionsList[2],texcoordsList[0] },
			{ positionsList[5],texcoordsList[2] },
			{ positionsList[4],texcoordsList[3] },

			//left face
			{ positionsList[7],texcoordsList[1] },
			{ positionsList[6],texcoordsList[0] },
			{ positionsList[1],texcoordsList[2] },
			{ positionsList[0],texcoordsList[3] }
		};

		UINT32 indicesList[] =
		{
			//front
			0,1,2,
			2,3,0,

			//back
			4,5,6,
			6,7,4,

			//top
			8,9,10,
			10,11,8,

			//bottom
			12,13,14,
			14,15,12,

			//right
			16,17,18,
			18,19,16,

			//left
			20,21,22,
			22,23,20
		};

		VertexAttribute attribList[] = {
			sizeof(glm::vec3) / sizeof(float),		// POSITION
			sizeof(glm::vec2) / sizeof(float)		// TEXCOORD
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
				(void*)indicesList,
				sizeof(indicesList)
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

		auto display = GetInnerSize();
		m_Camera = Graphics::Instance()->CreateCamera
		({
			90.f,
			(float)display.width(),
			(float)display.height(),
		});
		m_Camera->Translate(glm::vec3(0.0f, 2.0f, 8.0f));
		m_Camera->Update();
	}

	// Update
	VOID Update(float deltatime)
	{
		Graphics::Instance()->Clear(Vec4D(0.26f, 0.26f, 0.26f, 0.5f));

		m_UI->RenderUI(m_FrameBuffer->Textures());

		glm::mat4 u_ModelViewProjection = m_Camera->GetViewProj();
		UniformData data = { u_ModelViewProjection };
		m_Uniform->SetData(&data);

		m_FrameBuffer->Bind();

		Graphics::Instance()->Clear(Vec4D(0.3f, 0.3f, 0.7f, 1.f));
		
		Graphics::Instance()->SetFaceCulling(CullType::FRONT_FACE);
		Graphics::Instance()->SetWindingOrder(WindingOrder::CLOCKWISE);	
		Graphics::Instance()->SetVertexArrayObject(m_VAO);
		Graphics::Instance()->SetUniformBuffer(m_Uniform, 0);
		Graphics::Instance()->SetShaderProgram(m_Shader);
		
		Graphics::Instance()->DrawIndexedTriangles(TriangleType::TRIANGLE_LIST, 36);

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
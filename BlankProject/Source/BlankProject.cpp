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

		Vec3D positionsList[] =
		{
			//front face
			Vec3D(-0.5f,-0.5f,-0.5f),
			Vec3D(-0.5f,0.5f,-0.5f),
			Vec3D(0.5f,0.5f,-0.5f),
			Vec3D(0.5f,-0.5f,-0.5f),
			
			//back face
			Vec3D(0.5f,-0.5f,0.5f),
			Vec3D(0.5f,0.5f,0.5f),
			Vec3D(-0.5f,0.5f,0.5f),
			Vec3D(-0.5f,-0.5f,0.5f)
		};

		Vec2D texcoordsList[] = // RG Only
		{
			Vec2D(0,0),
			Vec2D(0,1),
			Vec2D(1,0),
			Vec2D(1,1)
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
			sizeof(Vec3D) / sizeof(FLOAT),		// POSITION
			sizeof(Vec2D) / sizeof(FLOAT)		// TEXCOORD
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
				indicesList,
				sizeof(indicesList)
			}
		);

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

		// init a framebuffer
		m_FrameBuffer = Graphics::Instance()->CreateFrameBuffer();
	}

	// Update
	VOID Update() 
	{
		Graphics::Instance()->Clear(Vec4D(0.26f, 0.26f, 0.26f, 0.5f));

		// render the ui before presenting the scene
		m_UI->RenderUI(m_FrameBuffer->Textures());

		// compute delta time
		auto currentTime = std::chrono::system_clock::now();
		auto elapsedSeconds = std::chrono::duration<double>();
		if (m_PreviousTime.time_since_epoch().count())
			elapsedSeconds = currentTime - m_PreviousTime;
		m_PreviousTime = currentTime;

		auto deltaTime = elapsedSeconds.count();

		m_Scale += 1.14f * deltaTime;
		auto currentScale = abs(sin(m_Scale));

		Mat4 world, projection, temp;

		temp.SetIdentity();
		temp.SetScale(Vec3D(1, 1, 1));
		world *= temp;

		temp.SetIdentity();
		temp.SetRotationX(m_Scale);
		world *= temp;

		temp.SetIdentity();
		temp.SetRotationY(m_Scale);
		world *= temp;

		temp.SetIdentity();
		temp.SetRotationZ(m_Scale);
		world *= temp;

		temp.SetIdentity();
		temp.SetTranslation(Vec3D(0, 0, 0));
		world *= temp;

		auto displaySize = Simulation::GetInnerSize();
		projection.SetOrthoLH(displaySize.width() * 0.004f, displaySize.height() * 0.004f, 0.01f, 100.f);

		m_FrameBuffer->Bind();

		Graphics::Instance()->Clear(Vec4D(0.3f, 0.3f, 0.7f, 1.f));

		UniformData data = { world, projection };
		m_Uniform->SetData(&data);
		
		Graphics::Instance()->SetFaceCulling(CullType::BACK_FACE);
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
#include "Ray.h"

UI::UI()
	: m_Handle(GetForegroundWindow())
{
	m_Context = ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	m_IO = io;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplWin32_Init(m_Handle, wglGetCurrentContext());
	ImGui_ImplOpenGL3_Init("#version 130");

	InitFileBrowser();
	m_AssetManager.Initialize("Asset Manager", AssetManagerFlags::NONE);

	m_Initialized = true;
}

UI::~UI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

VOID UI::RenderUI(UINT32 scene, UI::CameraType type)
{
	HGLRC context = wglGetCurrentContext();
	HDC hdc = wglGetCurrentDC();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	RenderElements(scene, type);

	ImGui::Render();

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	wglMakeCurrent(hdc, context);
}

RECT UI::SceneRect()
{
	RECT rect = { ScenePos().x, ScenePos().y, ScenePos().x + SceneSize().x , ScenePos().y + SceneSize().y };
	return rect;
}

VOID UI::RenderElements(UINT32 scene, UI::CameraType type)
{
	MakeWindowDockspace();

	RenderMainMenubar();
	RenderInfoMenu(type);
	RenderLogMenu();

	// API for graphics engine
	RenderPrimitiveCreationWindow();
	RenderPrimitiveUpdateWindow();

	// our asset manager
	m_AssetManager.Render();

	RenderScene(scene);
}

VOID UI::MakeWindowDockspace()
{
	// Render docking spaces on the window
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::SetNextWindowBgAlpha(0.0f);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", 0, window_flags);
	ImGui::PopStyleVar(3);

	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	ImGui::End();
}

VOID UI::RenderMainMenubar()
{
	// menu bar
	if (ImGui::BeginMainMenuBar())
	{

		ImGui::EndMainMenuBar();
	}
}

#pragma region Primitives
VOID UI::RenderPrimitiveCreationWindow()
{
	static PrimitiveDesc desc;

	ImGui::Begin("Add Primitive");
	{
		ImGui::Combo("Primitive Type", (int*)&m_CurrentPrimitive, m_PrimitiveTypes, IM_ARRAYSIZE(m_PrimitiveTypes));

		ImGui::InputTextWithHint("##Texture", "Texture", desc.m_Texture, MAX_PATH); ImGui::SameLine();
		if (ImGui::Button("..##TextureButton"))
			m_FileBrowser[BROWSER_TEXTURE].Open();
		ImGui::InputTextWithHint("##FragmentShader", "Fragment Shader", desc.m_FragmentShader, MAX_PATH); ImGui::SameLine();
		if (ImGui::Button("..##FragmentShaderButton"))
			m_FileBrowser[BROWSER_FRAGMENT].Open();
		ImGui::InputTextWithHint("##VertexShader", "Vertex Shader", desc.m_VertexShader, MAX_PATH); ImGui::SameLine();
		if (ImGui::Button("..##VertexShaderButton"))
			m_FileBrowser[BROWSER_VERTEX].Open();

		ImGui::InputFloat3("Pos", &desc.m_Pos.x);
		ImGui::InputFloat3("Rot", &desc.m_Rotation.x);
		ImGui::InputFloat3("Rot Vel", &desc.m_RotationVel.x);
		ImGui::InputFloat3("Vel", &desc.m_Velocity.x);
		ImGui::ColorEdit3("Color", &desc.m_Col[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

		if (ImGui::Button("Add Primitive", ImVec2(-1, 0)))
			AddPrimitive(desc);
	}
	ImGui::End();

	ManageFileBrowser(desc);
}

VOID UI::InitFileBrowser()
{
	m_FileBrowser[BROWSER_FRAGMENT].SetTitle("Fragment Shader");
	m_FileBrowser[BROWSER_FRAGMENT].SetTypeFilters({ ".fs", ".fragment", ".glsl", ".frag" });

	m_FileBrowser[BROWSER_VERTEX].SetTitle("Vertex Shader");
	m_FileBrowser[BROWSER_VERTEX].SetTypeFilters({ ".vs", ".vertex", ".glsl", ".vert" });

	m_FileBrowser[BROWSER_TEXTURE].SetTitle("Texture");
	m_FileBrowser[BROWSER_TEXTURE].SetTypeFilters({ ".jpg", ".png", ".jpeg", ".bmp" });
}

VOID UI::ManageFileBrowser(PrimitiveDesc& desc)
{
	for (int i = 0; i < 3; i++)
	{
		m_FileBrowser[i].Display();
		if (m_FileBrowser[i].HasSelected())
		{
			switch (i)
			{
			case BROWSER_FRAGMENT:
				ZeroMemory(desc.m_FragmentShader, MAX_PATH);
				memcpy(desc.m_FragmentShader, m_FileBrowser[i].GetSelected().string().c_str(), m_FileBrowser[i].GetSelected().string().size());
				break;
			case BROWSER_VERTEX:
				ZeroMemory(desc.m_VertexShader, MAX_PATH);
				memcpy(desc.m_VertexShader, m_FileBrowser[i].GetSelected().string().c_str(), m_FileBrowser[i].GetSelected().string().size());
				break;
			case BROWSER_TEXTURE:
				ZeroMemory(desc.m_Texture, MAX_PATH);
				memcpy(desc.m_Texture, m_FileBrowser[i].GetSelected().string().c_str(), m_FileBrowser[i].GetSelected().string().size());
				break;
			}
		}
	}
}

VOID UI::AddPrimitive(const PrimitiveDesc& desc)
{
	Graphics::Instance()->CreatePrimitive(m_CurrentPrimitive, desc);
}

VOID UI::RenderPrimitiveUpdateWindow()
{
	/*
	ImGui::Begin("Primitives");
	{
		if (ImGui::CollapsingHeader("Cube(s)"))
		{
			int primcount = 0;
			for (auto& e : PrimitiveContainer::Instance().m_Cubes)
			{
				std::string name = "Cube" + std::to_string(primcount);
				if (ImGui::TreeNode(name.c_str()))
				{
					glm::vec3 realPos = e.m_Description.m_Pos + e.GetTranslationScale();
					if (ImGui::InputFloat3("Pos", &realPos.x))
					{
						e.m_Description.m_Pos = realPos;
						e.SetTranslationScale(glm::vec3(0.f));
					}

					glm::vec3 realRotation = e.m_Description.m_Rotation + e.GetRotationScale();
					if (ImGui::InputFloat3("Rot", &realRotation.x))
					{
						e.m_Description.m_Rotation = realRotation;
						e.SetRotationScale(glm::vec3(0.f));
					}

					ImGui::InputFloat3("Vel", &e.m_Description.m_Velocity.x);
					ImGui::InputFloat3("Rot Vel", &e.m_Description.m_RotationVel.x);
					ImGui::TreePop();
					ImGui::Separator();
				}
				primcount++;
			}
		}
		if (ImGui::CollapsingHeader("Pyramid(s)"))
		{
			int primcount = 0;
			for (auto& e : PrimitiveContainer::Instance().m_Pyramids)
			{
				std::string name = "Pyramid" + std::to_string(primcount);
				if (ImGui::TreeNode(name.c_str()))
				{
					glm::vec3 realPos = e.m_Description.m_Pos + e.GetTranslationScale();
					if (ImGui::InputFloat3("Pos", &realPos.x))
					{
						e.m_Description.m_Pos = realPos;
						e.SetTranslationScale(glm::vec3(0.f));
					}

					glm::vec3 realRotation = e.m_Description.m_Rotation + e.GetRotationScale();
					if (ImGui::InputFloat3("Rot", &realRotation.x))
					{
						e.m_Description.m_Rotation = realRotation;
						e.SetRotationScale(glm::vec3(0.f));
					}

					ImGui::InputFloat3("Vel", &e.m_Description.m_Velocity.x);
					ImGui::InputFloat3("Rot Vel", &e.m_Description.m_RotationVel.x);
					ImGui::TreePop();
					ImGui::Separator();
				}
				primcount++;
			}
		}
		if (ImGui::CollapsingHeader("Square(s)"))
		{
			int primcount = 0;
			for (auto& e : PrimitiveContainer::Instance().m_Squares)
			{
				std::string name = "Square" + std::to_string(primcount);
				if (ImGui::TreeNode(name.c_str()))
				{
					glm::vec3 realPos = e.m_Description.m_Pos + e.GetTranslationScale();
					if (ImGui::InputFloat3("Pos", &realPos.x))
					{
						e.m_Description.m_Pos = realPos;
						e.SetTranslationScale(glm::vec3(0.f));
					}

					glm::vec3 realRotation = e.m_Description.m_Rotation + e.GetRotationScale();
					if (ImGui::InputFloat3("Rot", &realRotation.x))
					{
						e.m_Description.m_Rotation = realRotation;
						e.SetRotationScale(glm::vec3(0.f));
					}

					ImGui::InputFloat3("Vel", &e.m_Description.m_Velocity.x);
					ImGui::InputFloat3("Rot Vel", &e.m_Description.m_RotationVel.x);
					ImGui::TreePop();
					ImGui::Separator();
				}
				primcount++;
			}
		}
		if (ImGui::CollapsingHeader("Triangle(s)"))
		{
			int primcount = 0;
			for (auto& e : PrimitiveContainer::Instance().m_Triangles)
			{
				std::string name = "Triangle" + std::to_string(primcount);
				if (ImGui::TreeNode(name.c_str()))
				{
					glm::vec3 realPos = e.m_Description.m_Pos + e.GetTranslationScale();
					if (ImGui::InputFloat3("Pos", &realPos.x))
					{
						e.m_Description.m_Pos = realPos;
						e.SetTranslationScale(glm::vec3(0.f));
					}

					glm::vec3 realRotation = e.m_Description.m_Rotation + e.GetRotationScale();
					if (ImGui::InputFloat3("Rot", &realRotation.x))
					{
						e.m_Description.m_Rotation = realRotation;
						e.SetRotationScale(glm::vec3(0.f));
					}

					ImGui::InputFloat3("Vel", &e.m_Description.m_Velocity.x);
					ImGui::InputFloat3("Rot Vel", &e.m_Description.m_RotationVel.x);
					ImGui::TreePop();
					ImGui::Separator();
				}
				primcount++;
			}
		}
	}
	ImGui::End();*/
}
#pragma endregion

VOID UI::RenderInfoMenu(UI::CameraType type)
{
	glm::vec3 pos(0.f); glm::vec2 view(0.f);
	switch (type)
	{
	case CameraType::CAMERA_NORMAL:
		pos = Camera::GetCam().Position();
		break;
	case CameraType::CAMERA_FPS:
		pos = FPSCamera::GetFPSCam().Position();
		view = FPSCamera::GetFPSCam().View();
		break;
	case CameraType::CAMERA_FLOATING:
		pos = FloatingCamera::GetFloatingCam().Position();
		view = FloatingCamera::GetFloatingCam().View();
		break;
	}

	ImGui::Begin("Info");
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Scene Ratio %.0f/%.0f = %f", m_SceneSize.x, m_SceneSize.y, m_SceneSize.x / m_SceneSize.y);
	ImGui::Text("Cam pos: x:%.1f y:%.1f z:%.1f", pos.x, pos.y, pos.z);
	ImGui::Text("View: pitch:%.1f yaw:%.1f", view.x, view.y);
	ImGui::End();
}

VOID UI::RenderScene(UINT32 scene)
{
	// our scene window
	ImGui::Begin("Scene", 0);
	{
		m_SceneActive = ImGui::IsWindowFocused();

		ImVec2 windowsize = ImGui::GetWindowSize();
		ImVec2 windowpos = ImGui::GetWindowPos();

		m_SceneSize = glm::vec2(windowsize.x, windowsize.y);
		m_ScenePos = glm::vec2(windowpos.x, windowpos.y);

		ImVec2 wsize = ImGui::GetContentRegionAvail();
		ImGui::Image((ImTextureID)scene, wsize, ImVec2(0, 1), ImVec2(1, 0));
	}
	ImGui::End();
}

VOID UI::RenderLogMenu()
{
	bool b = false;

	ImGui::Begin("Logs");
	{
		ImGui::ListBoxHeader("##LogBox", ImVec2(-1,-1));
		for (auto ws : Logger::Instance()->m_Logs)
		{
			std::string str;
			std::transform(ws.begin(), ws.end(), std::back_inserter(str), [](wchar_t c) {
				return (char)c;
			});
			ImGui::Selectable(str.c_str(), b);
		}
		ImGui::ListBoxFooter();
	}
	ImGui::End();
}
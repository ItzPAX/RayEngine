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
	RenderSceneTreeView();
	RenderPointLightCreationWindow();
	RenderPrimitiveCreationWindow();
	RenderPrimitiveUpdateWindow();
	RenderLightUpdateWindow();
	RenderModelUpdateWindow();

	// MenuBar
	RenderMetricsWindow();
	RenderAboutWindow();

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
	ImGui::Begin("DD", 0, window_flags);
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
		if (ImGui::BeginMenu("View"))
		{
			ImGui::MenuItem("Asset Manager", NULL, &m_AssetManager.m_AssetManager);
			ImGui::MenuItem("Primitive Creation", NULL, &m_PrimitiveCreation);
			ImGui::MenuItem("Primitive Editor", NULL, &m_PrimitiveEditor);
			ImGui::MenuItem("Model Editor", NULL, &m_ModelEditor);
			ImGui::MenuItem("Light Creation", NULL, &m_LightCreation);
			ImGui::MenuItem("Light Editor", NULL, &m_LightEditor);
			ImGui::MenuItem("Info", NULL, &m_Info);
			ImGui::MenuItem("Log", NULL, &m_Log);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("Metrics/Debugger", NULL, &m_MetricsActive);
			ImGui::Separator();
			ImGui::MenuItem("About RayEngine", NULL, &m_AboutActive);
			ImGui::EndMenu();
		}
	}
}

#pragma region Primitives
inline void UI::RenderPrimitiveDescriptionEditor(PrimitiveDesc* pdesc, MaterialDesc* pmat)
{
	ImGui::InputTextWithHint("##Texture", "Texture", pdesc->m_Texture, MAX_PATH); ImGui::SameLine();
	if (ImGui::Button("..##TextureButton"))
		m_FileBrowser[BROWSER_TEXTURE].Open();
	ImGui::InputTextWithHint("##FragmentShader", "Fragment Shader", pdesc->m_FragmentShader, MAX_PATH); ImGui::SameLine();
	if (ImGui::Button("..##FragmentShaderButton"))
		m_FileBrowser[BROWSER_FRAGMENT].Open();
	ImGui::InputTextWithHint("##VertexShader", "Vertex Shader", pdesc->m_VertexShader, MAX_PATH); ImGui::SameLine();
	if (ImGui::Button("..##VertexShaderButton"))
		m_FileBrowser[BROWSER_VERTEX].Open();

	ImGui::InputFloat3("Position", &pdesc->m_Position.x);
	ImGui::InputFloat3("Rotation", &pdesc->m_Rotation.x);
	ImGui::InputFloat3("Velocity", &pdesc->m_Velocity.x);
	ImGui::InputFloat3("Rotation Velocity", &pdesc->m_RotationVel.x);

	ImGui::ColorEdit3("Object Color", &pdesc->m_ObjectColor[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
	ImGui::ColorEdit3("Ambient", &pmat->m_Ambient[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
	ImGui::ColorEdit3("Diffuse", &pmat->m_Diffuse[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
	ImGui::ColorEdit3("Specular", &pmat->m_Specular[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
	ImGui::InputFloat("Shininess", &pmat->m_Shininess);
}

VOID UI::RenderPrimitiveCreationWindow()
{
	if (!m_PrimitiveCreation)
		return;

	static PrimitiveDesc desc;
	static MaterialDesc mat;

	ImGui::Begin("Add Primitive", &m_PrimitiveCreation);
	{
		ImGui::Combo("Primitive Type", (int*)&m_CurrentPrimitive, m_PrimitiveTypes, IM_ARRAYSIZE(m_PrimitiveTypes));
		RenderPrimitiveDescriptionEditor(&desc, &mat);
		if (ImGui::Button("Add Primitive", ImVec2(-1, 0)))
			AddPrimitive(desc, mat);
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

VOID UI::AddPrimitive(const PrimitiveDesc& desc, const MaterialDesc& material)
{
	// will automatically discard itself
	PrimitivePtr prim = Graphics::Instance()->CreatePrimitive(m_CurrentPrimitive, desc, material);
}

VOID UI::RenderMetricsWindow()
{
	if (!m_MetricsActive)
		return;
	
	auto io = ImGui::GetIO();

	ImGui::Begin("Metrics/Debugger", &m_MetricsActive, ImGuiWindowFlags_NoDocking);
	ImGui::Text("Engine Metrics");
	ImGui::Text("Vertices this frame: %d", Graphics::Instance()->m_TrianglesThisFrame);
	ImGui::Text("Draw calls this frame: %d", Graphics::Instance()->m_DrawCallsThisFrame);
	ImGui::Separator();

	ImGui::Text("Dear ImGui %s", ImGui::GetVersion());
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
	ImGui::Text("%d vertices, %d indices (%d triangles)", io.MetricsRenderVertices, io.MetricsRenderIndices, io.MetricsRenderIndices / 3);
	ImGui::Text("%d visible windows, %d active allocations", io.MetricsRenderWindows, io.MetricsActiveAllocations);
	ImGui::End();
}

VOID UI::RenderAboutWindow()
{
	if (!m_AboutActive)
		return;

	auto io = ImGui::GetIO();

	ImGui::Begin("About", &m_AboutActive, ImGuiWindowFlags_NoDocking);
	ImGui::Text("About RayEngine");
	ImGui::Separator();
	ImGui::TextWrapped("RayEngine is developed by ItzPAX and is a FOSS engine that uses the OpenGL 4.6 rendering pipeline\n");
	ImGui::End();
}

template <typename T>
inline VOID UI::RenderPrimitiveTreeView(std::string primitive, PRIMITIVE_TYPE type)
{
	if (ImGui::CollapsingHeader((primitive + "(s)").c_str()))
	{
		int primcount = 0;
		for (auto& entry : SceneContainer::Instance().GetActiveScene()->GetPrimitiveContainer()->m_Primitives)
		{
			auto& e = entry.second;
			if (e.m_Type != type)
				continue;
			T* c = (T*)e.m_Data;

			std::string name = primitive + std::to_string(primcount);
			if (ImGui::TreeNode(name.c_str()))
			{
				RenderPrimitiveDescriptionEditor(c->GetDescription(), c->GetMaterial());
				if (ImGui::Button("Delete"))
				{
					c->Delete();
					break;
				}
				ImGui::TreePop();
				ImGui::Separator();
			}
			primcount++;
		}
	}
}

VOID UI::RenderPrimitiveUpdateWindow()
{
	if (!m_PrimitiveEditor)
		return;

	ImGui::Begin("Primitives", &m_PrimitiveEditor);
	{
		RenderPrimitiveTreeView<Cube>("Cube", PRIMITIVE_TYPE::PRIMITIVE_CUBE);
		RenderPrimitiveTreeView<Pyramid>("Pyramid", PRIMITIVE_TYPE::PRIMITIVE_PYRAMID);
		RenderPrimitiveTreeView<Square>("Square", PRIMITIVE_TYPE::PRIMITIVE_SQUARE);
		RenderPrimitiveTreeView<Triangle>("Triangle", PRIMITIVE_TYPE::PRIMITIVE_TRIANGLE);
	}
	ImGui::End();
}
#pragma endregion

#pragma region Light
VOID UI::RenderSpotLightTreeView()
{
	if (ImGui::CollapsingHeader("Spotlight"))
	{
		ImGui::InputFloat3("Position##Spotlight", &SceneContainer::Instance().GetActiveScene()->GetLightingManager()->m_SpotLight.m_Position.x);
		ImGui::InputFloat3("Direction##Spotlight", &SceneContainer::Instance().GetActiveScene()->GetLightingManager()->m_SpotLight.m_Direction.x);
		ImGui::InputFloat("Outer cut off##Spotlight", &SceneContainer::Instance().GetActiveScene()->GetLightingManager()->m_SpotLight.m_OuterCutOff);
		ImGui::InputFloat("Inner cut off##Spotlight", &SceneContainer::Instance().GetActiveScene()->GetLightingManager()->m_SpotLight.m_InnerCutOff);

		ImGui::ColorEdit3("Ambient##Spotlight", &SceneContainer::Instance().GetActiveScene()->GetLightingManager()->m_SpotLight.m_Ambient[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
		ImGui::ColorEdit3("Diffuse##Spotlight", &SceneContainer::Instance().GetActiveScene()->GetLightingManager()->m_SpotLight.m_Diffuse[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
		ImGui::ColorEdit3("Specular##Spotlight", &SceneContainer::Instance().GetActiveScene()->GetLightingManager()->m_SpotLight.m_Specular[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

		ImGui::InputFloat("Constant##Spotlight", &SceneContainer::Instance().GetActiveScene()->GetLightingManager()->m_SpotLight.m_Constant);
		ImGui::InputFloat("Linear##Spotlight", &SceneContainer::Instance().GetActiveScene()->GetLightingManager()->m_SpotLight.m_Linear);
		ImGui::InputFloat("Quadratic##Spotlight", &SceneContainer::Instance().GetActiveScene()->GetLightingManager()->m_SpotLight.m_Quadratic);

	}
}
VOID UI::RenderDirectionalLightTreeView()
{
	if (ImGui::CollapsingHeader("Directional light"))
	{
		ImGui::InputFloat3("Direction##DirectionalLight", &SceneContainer::Instance().GetActiveScene()->GetLightingManager()->m_DirectionalLight.m_Direction.x);

		ImGui::ColorEdit3("Ambient##DirectionalLight", &SceneContainer::Instance().GetActiveScene()->GetLightingManager()->m_DirectionalLight.m_Ambient[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
		ImGui::ColorEdit3("Diffuse##DirectionalLight", &SceneContainer::Instance().GetActiveScene()->GetLightingManager()->m_DirectionalLight.m_Diffuse[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
		ImGui::ColorEdit3("Specular##DirectionalLight", &SceneContainer::Instance().GetActiveScene()->GetLightingManager()->m_DirectionalLight.m_Specular[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
	}
}

VOID UI::RenderPointLightEditor(PointLight& pl)
{
	ImGui::InputFloat3("Position##PointLight", &pl.m_Position.x);

	ImGui::ColorEdit3("Ambient##PointLight", &pl.m_Ambient[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
	ImGui::ColorEdit3("Diffuse##PointLight", &pl.m_Diffuse[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
	ImGui::ColorEdit3("Specular##PointLight", &pl.m_Specular[0], ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);

	ImGui::InputFloat("Constant##PointLight", &pl.m_Constant);
	ImGui::InputFloat("Linear##PointLight", &pl.m_Linear);
	ImGui::InputFloat("Quadratic##PointLight", &pl.m_Quadratic);
}

VOID UI::RenderPointLightTreeView()
{
	if (ImGui::CollapsingHeader("Point light(s)"))
	{
		int light = 0;
		for (auto& pointlight_map : SceneContainer::Instance().GetActiveScene()->GetLightingManager()->m_PointLights)
		{
			std::string name = "PointLight" + std::to_string(light);
			if (ImGui::TreeNode(name.c_str()))
			{
				RenderPointLightEditor(pointlight_map.second);

				if (ImGui::Button("Delete"))
				{
					SceneContainer::Instance().GetActiveScene()->GetLightingManager()->DeletePointLight(pointlight_map.second);
					break;
				}

				ImGui::TreePop();
				ImGui::Separator();
			}
			light++;
		}
	}
}

VOID UI::RenderLightUpdateWindow()
{
	if (!m_LightEditor)
		return;

	ImGui::Begin("Lights", &m_LightEditor);
	{
		RenderSpotLightTreeView();
		RenderDirectionalLightTreeView();
		RenderPointLightTreeView();
	}
	ImGui::End();
}

VOID UI::RenderModelTreeView()
{
	if (ImGui::CollapsingHeader("Model(s)"))
	{
		int model = 0;
		for (auto& mptr_map : SceneContainer::Instance().GetActiveScene()->GetModelContainer()->m_Models)
		{
			std::string name = "Model" + std::to_string(model);
			if (ImGui::TreeNode(name.c_str()))
			{
				ImGui::InputFloat3(("Position##" + name).c_str(), &mptr_map.second->m_Position.x);
				ImGui::InputFloat3(("Rotation##" + name).c_str(), &mptr_map.second->m_Rotation.x);
				ImGui::InputFloat(("Scale##" + name).c_str(), &mptr_map.second->m_Scale);

				ImGui::Text("Model Info");
				ImGui::Text("Meshes: %d", mptr_map.second->m_Meshes.size());
				ImGui::Text("Unique Textures: %d", mptr_map.second->m_TexturesLoaded.size());

				if (ImGui::Button("Delete"))
				{
					SceneContainer::Instance().GetActiveScene()->GetModelContainer()->Delete(mptr_map.second);
					break;
				}

				ImGui::TreePop();
				ImGui::Separator();
			}
			model++;
		}
	}
}

VOID UI::RenderModelUpdateWindow()
{
	if (!m_ModelEditor)
		return;

	ImGui::Begin("Model", &m_ModelEditor);
	{
		RenderModelTreeView();
	}
	ImGui::End();
}

VOID UI::RenderSceneTreeView()
{
	if (ImGui::Begin("Scene Overview"))
	{
		if (ImGui::CollapsingHeader("Scene(s)"))
		{
			for (auto& scene_map : SceneContainer::Instance().m_AllScenes)
			{
				if (ImGui::TreeNode(scene_map.first.c_str()))
				{
					if (ImGui::Button("Set Active"))
						SceneContainer::Instance().SetActiveScene(scene_map.second);
					if (ImGui::Button("Delete"))
					{
						SceneContainer::Instance().RemoveScene(scene_map.second);

						if (!SceneContainer::Instance().m_AllScenes.empty())
							SceneContainer::Instance().SetActiveScene(SceneContainer::Instance().m_AllScenes.begin()->second);
						else
							SceneContainer::Instance().SetActiveScene("NO_SCENE");

						break;
					}

					ImGui::TreePop();
					ImGui::Separator();
				}
			}
		}
	}
}

VOID UI::RenderPointLightCreationWindow()
{
	if (!m_LightCreation)
		return;

	static PointLight light;

	ImGui::Begin("Add PointLight", &m_LightCreation);
	{
		RenderPointLightEditor(light);
		if (ImGui::Button("Add Light", ImVec2(-1, 0)))
			Graphics::Instance()->CreatePointLight(light);
	}
	ImGui::End();
}
#pragma endregion

VOID UI::RenderInfoMenu(UI::CameraType type)
{
	if (!m_Info)
		return;

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

	ImGui::Begin("Info", &m_Info);
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Vertices this frame: %d", Graphics::Instance()->m_TrianglesThisFrame);
	ImGui::Text("Draw calls this frame: %d", Graphics::Instance()->m_DrawCallsThisFrame);

	ImGui::NewLine();

	ImGui::Text("Scene Ratio %.0f/%.0f = %f", m_SceneSize.x, m_SceneSize.y, m_SceneSize.x / m_SceneSize.y);
	ImGui::Text("Cam pos: x:%.1f y:%.1f z:%.1f", pos.x, pos.y, pos.z);
	ImGui::Text("View: pitch:%.1f yaw:%.1f", view.x, view.y);
	ImGui::End();
}

VOID UI::RenderScene(UINT32 scene)
{
	// our scene window
	ImGui::Begin(SceneContainer::Instance().GetActiveScene()->GetName().c_str(), 0);
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
	if (!m_Log)
		return;

	bool b = false;

	ImGui::Begin("Logs", &m_Log);
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
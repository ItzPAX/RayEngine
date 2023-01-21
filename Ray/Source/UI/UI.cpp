#include "Ray.h"

UI::UI(HWND handle)
	: m_Handle(handle)
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

	m_AssetManager.Initialize("Asset Manager", AssetManagerFlags::NONE);

	m_Initialized = true;
}

UI::~UI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

VOID UI::RenderUI(UINT32 scene, const CameraInfo& caminfo)
{
	HGLRC context = wglGetCurrentContext();
	HDC hdc = wglGetCurrentDC();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	RenderElements(scene, caminfo);

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

VOID UI::RenderElements(UINT32 scene, const CameraInfo& caminfo)
{
	MakeWindowDockspace();

	RenderMainMenubar();
	RenderInfoMenu(caminfo);
	RenderLogMenu();

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
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
			if (ImGui::MenuItem("Close", "Ctrl+W")) { /* Do stuff */ }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

VOID UI::RenderInfoMenu(const CameraInfo& caminfo)
{
	auto context = wglGetCurrentContext();
	ImGui::Begin("Info");
	ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("Scene Ratio %f", m_SceneSize.x / m_SceneSize.y);
	ImGui::Text("Context 0x%x", context);
	ImGui::Text("Cam pos: x:%.1f y:%.1f z:%.1f", caminfo.m_Pos.x, caminfo.m_Pos.y, caminfo.m_Pos.z);
	ImGui::Text("View: pitch:%.1f yaw:%.1f", caminfo.m_Pitch, caminfo.m_Yaw);
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
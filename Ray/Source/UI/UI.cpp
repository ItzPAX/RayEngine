#include "Ray.h"

UI::UI(HWND handle)
{
	m_Handle = handle;
	Initialize();
}

UI::~UI()
{
}

VOID UI::Initialize()
{
	m_Context = ImGui::CreateContext();
	ImGui_ImplWin32_Init(m_Handle);
	ImGui_ImplOpenGL3_Init("#version 460");
}

VOID UI::RenderUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	RenderElements();

	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

VOID UI::SetContext()
{
	ImGui::SetCurrentContext(m_Context);
}

static bool b = true;
VOID UI::RenderElements()
{
	// Create a window called "My First Tool", with a menu bar.
	ImGui::Begin("My First Tool", &b, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_UnsavedDocument);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
			if (ImGui::MenuItem("Close", "Ctrl+W")) { b = false; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// Edit a color (stored as ~4 floats)
	ImGui::ColorEdit4("Color", my_color);

	// Plot some values
	const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
	ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

	// Display contents in a scrolling region
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
	ImGui::BeginChild("Scrolling");
	for (int n = 0; n < 50; n++)
		ImGui::Text("%04d: Some text", n);
	ImGui::EndChild();
	ImGui::End();
}

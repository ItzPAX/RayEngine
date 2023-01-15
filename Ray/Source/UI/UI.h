#pragma once

class RAY_API UI
{
	//Constructor
public:
	UI(HWND handle);
	~UI();

public:
	VOID Initialize();
	VOID RenderUI();
	VOID SetContext();

private:
	VOID RenderElements();

private:
	bool my_tool_active = true;
	float my_color[4] = { 1,1,1,1 };

	ImGuiContext* m_Context;
	HWND m_Handle;
};
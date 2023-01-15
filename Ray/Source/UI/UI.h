#pragma once

class RAY_API UI
{
	//Constructor
public:
	UI(HWND handle);
	~UI();

public:
	VOID RenderUI();
	VOID SetContext();

private:
	VOID RenderElements();

private:
	bool m_Initialized = false;

	std::vector<float> m_Frametimes;
	std::vector<float> m_TmpTimes;

	ImGuiContext* m_Context;
	HWND m_Handle;
};
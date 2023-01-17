#pragma once

class RAY_API UI
{
	//Constructor
public:
	UI(HWND handle);
	~UI();

public:
	/* PARAM: the textures stored in the FBO */
	VOID RenderUI(UINT32 scene);

	// getters
public:
	Vec2D SceneSize() { return m_SceneSize; }

private:
	VOID RenderElements(UINT32 scene);
	
	VOID MakeWindowDockspace();
	VOID RenderMainMenubar();
	VOID RenderScene(UINT32 scene);
	VOID RenderInfoMenu();
	VOID RenderLogMenu();

private:
	bool m_Initialized = false;
	Vec2D m_SceneSize;

	AssetManager m_AssetManager;

	ImGuiContext* m_Context;
	HWND m_Handle;
};
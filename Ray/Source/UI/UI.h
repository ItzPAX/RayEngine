#pragma once
#pragma warning( disable : 4312 )

struct CameraInfo
{
	glm::vec3 m_Pos;
	float m_Pitch;
	float m_Yaw;
};

class RAY_API UI
{
	//Constructor
public:
	UI(HWND handle);
	~UI();

public:
	VOID RenderUI(UINT32 scene, const CameraInfo& caminfo);
	RECT SceneRect();

	// getters
public:
	glm::vec2 SceneSize() { return m_SceneSize; }
	glm::vec2 ScenePos() { return m_ScenePos; }
	ImGuiIO& IO() { return m_IO; };
	bool SceneActive() { return m_SceneActive; }

private:
	VOID RenderElements(UINT32 scene, const CameraInfo& caminfo);
	
	VOID MakeWindowDockspace();
	VOID RenderMainMenubar();
	VOID RenderScene(UINT32 scene);
	VOID RenderInfoMenu(const CameraInfo& caminfo);
	VOID RenderLogMenu();

private:
	bool m_Initialized = false;
	bool m_SceneActive = false;

	glm::vec2 m_SceneSize;
	glm::vec2 m_ScenePos;

	AssetManager m_AssetManager;

	ImGuiContext* m_Context;
	ImGuiIO m_IO;
	HWND m_Handle;
};
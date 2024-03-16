#pragma once
#pragma warning( disable : 4312 )

struct PrimitiveDesc;

class RAY_API UI
{
public:
	enum class CameraType
	{
		CAMERA_NORMAL,
		CAMERA_FPS,
		CAMERA_FLOATING
	};

public:
	static UI& Instance()
	{
		static UI instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}

public:
	UI(UI const&) = delete;
	void operator=(UI const&) = delete;

	//Constructor
public:
	UI();
	~UI();

public:
	VOID RenderUI(UINT32 scene, UI::CameraType type);
	RECT SceneRect();

	// getters
public:
	glm::vec2 SceneSize() { return m_SceneSize; }
	glm::vec2 ScenePos() { return m_ScenePos; }
	ImGuiIO& IO() { return m_IO; };
	bool SceneActive() { return m_SceneActive; }

private:
	VOID RenderElements(UINT32 scene, UI::CameraType type);
	
	VOID MakeWindowDockspace();
	VOID RenderMainMenubar();
	VOID RenderScene(UINT32 scene);
	VOID RenderInfoMenu(UI::CameraType type);
	VOID RenderLogMenu();

#pragma region Primitives
	enum BrowserType
	{
		BROWSER_VERTEX = 0,
		BROWSER_FRAGMENT,
		BROWSER_TEXTURE
	};

	void RenderPrimitiveDescriptionEditor(PrimitiveDesc* pdesc, MaterialDesc* pmat);

	template <typename T>
	VOID RenderPrimitiveTreeView(std::string primitive, PRIMITIVE_TYPE type);
	VOID RenderPrimitiveUpdateWindow();

	VOID RenderSpotLightTreeView();
	VOID RenderDirectionalLightTreeView();
	VOID RenderPointLightEditor(PointLight& pl);
	VOID RenderPointLightTreeView();
	VOID RenderLightUpdateWindow();

	VOID RenderPointLightCreationWindow();

	VOID RenderPrimitiveCreationWindow();
	VOID AddPrimitive(const PrimitiveDesc& desc, const MaterialDesc& material);

	VOID InitFileBrowser();
	VOID ManageFileBrowser(PrimitiveDesc& desc);
#pragma endregion

private:
	bool m_Initialized = false;
	bool m_SceneActive = false;

	PRIMITIVE_TYPE m_CurrentPrimitive;

	glm::vec2 m_SceneSize;
	glm::vec2 m_ScenePos;

	AssetManager m_AssetManager;
	ImGui::FileBrowser m_FileBrowser[3];

	ImGuiContext* m_Context;
	ImGuiIO m_IO;
	HWND m_Handle;

	const char* m_PrimitiveTypes[4] =
	{
		"Cube",
		"Pyramid",
		"Square",
		"Triangle"
	};

	const char* m_LightTypes[3] =
	{
		"Point",
		"Directional",
		"Spotlight"
	};
};
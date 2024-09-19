#pragma once

class RAY_API SceneContainer
{
public:
	static SceneContainer& Instance()
	{
		static SceneContainer instance;	   // Guaranteed to be destroyed.
		// Instantiated on first use.
		return instance;
	}
private:
	SceneContainer() {}                    // Constructor? (the {} brackets) are needed here.

public:
	SceneContainer(SceneContainer const&) = delete;
	void operator=(SceneContainer const&) = delete;

private:
	ScenePtr m_ActiveScene;
	std::unordered_map<STRING, ScenePtr> m_AllScenes;

public:
	ScenePtr GetActiveScene();
	ScenePtr GetScene(STRING scene_name);

	void SetActiveScene(ScenePtr scene);
	void SetActiveScene(STRING scene_name);

	void AddScene(ScenePtr scene);
	void RemoveScene(ScenePtr scene);
};

class RAY_API Scene
{
private:
	// buffer we draw to
	FrameBufferPtr m_FrameBuffer;

	// lighting for the scene
	LightingManagerPtr m_LightingManager;

	// objects in the scene
	ModelContainerPtr m_Models;
	PrimitiveContainerPtr m_Primitives;

	STRING m_Name;
	bool m_ValidScene;

public:
	Scene() = default;
	Scene(STRING name);

public:
	STRING GetName();

	FrameBufferPtr GetFrameBuffer();

	LightingManagerPtr GetLightingManager();

	ModelContainerPtr GetModelContainer();
	PrimitiveContainerPtr GetPrimitiveContainer();

public:
	bool IsValid();
};
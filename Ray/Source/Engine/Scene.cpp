#include "Ray.h"

ScenePtr SceneContainer::GetActiveScene()
{
	// if we dont have an active scene log a warning and create one
	if (!m_ActiveScene || !m_ActiveScene->IsValid())
	{
		ScenePtr scene = Graphics::Instance()->CreateScene("Scene");
		SetActiveScene(scene);

		Logger::Instance()->PrintOGL3DWarning(L"No active scene found! Creating one automatically...");
	}

	return m_ActiveScene;
}

ScenePtr SceneContainer::GetScene(STRING scene_name)
{
	if (m_AllScenes.find(scene_name) != m_AllScenes.end())
	{
		return m_AllScenes[scene_name];
	}

	std::wstringstream m;
	std::wstring name_tmp(scene_name.begin(), scene_name.end());
	m << "Scene " << name_tmp << " not found when getting scene!";
	Logger::Instance()->PrintOGL3DWarning(m.str().c_str());

	return nullptr;
}

void SceneContainer::SetActiveScene(ScenePtr scene)
{
	m_ActiveScene = scene;
}

void SceneContainer::SetActiveScene(STRING scene_name)
{
	if (m_AllScenes.find(scene_name) != m_AllScenes.end())
	{
		m_ActiveScene = m_AllScenes[scene_name];
		return;
	}

	std::wstringstream m;
	std::wstring name_tmp(scene_name.begin(), scene_name.end());
	m << "Scene " << name_tmp << " not found when setting active scene!";
	Logger::Instance()->PrintOGL3DWarning(m.str().c_str());
}

void SceneContainer::AddScene(ScenePtr scene)
{
	m_AllScenes[scene->GetName()] = scene;
}

void SceneContainer::RemoveScene(ScenePtr scene)
{
	m_AllScenes.erase(scene->GetName());
}

Scene::Scene(STRING name)
{
	m_FrameBuffer = Graphics::Instance()->CreateFrameBuffer(FrameBufferDescription());

	m_LightingManager = Graphics::Instance()->CreateLightingManager();

	m_Models = Graphics::Instance()->CreateModelContainer();
	m_Primitives = Graphics::Instance()->CreatePrimitiveContainer();

	m_Name = name;
	m_ValidScene = true;
}

STRING Scene::GetName()
{
	return m_Name;
}

FrameBufferPtr Scene::GetFrameBuffer()
{
	return m_FrameBuffer;
}

LightingManagerPtr Scene::GetLightingManager()
{
	return m_LightingManager;
}

ModelContainerPtr Scene::GetModelContainer()
{
	return m_Models;
}

PrimitiveContainerPtr Scene::GetPrimitiveContainer()
{
	return m_Primitives;
}

bool Scene::IsValid()
{
	return m_ValidScene;
}
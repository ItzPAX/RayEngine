#include "Ray.h"

ScenePtr SceneContainer::GetActiveScene()
{
	// if we dont have an active scene log a warning and create one
	if (!m_ActiveScene || !m_ActiveScene->IsValid())
	{
		Logger::Instance()->PrintOGL3DWarning(L"No active scene found! Creating one automatically...");

		ScenePtr scene = Graphics::Instance()->CreateScene("Scene");
		SetActiveScene(scene);
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
	m << "Scene " << name_tmp << " not found when setting active scene! Creating backup scene...";
	Logger::Instance()->PrintOGL3DWarning(m.str().c_str());

	ScenePtr scene = Graphics::Instance()->CreateScene("Scene");
	SetActiveScene(scene);
}

void SceneContainer::AddScene(ScenePtr scene)
{
	m_AllScenes[scene->GetName()] = scene;
}

void SceneContainer::RemoveScene(ScenePtr scene)
{
	scene->ClearScene();
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

	m_LightingManager->m_SpotLight = SpotLight{
		glm::vec3(1.f, 1.f, 1.f),			// position
		glm::vec3(1.f, 1.f, 1.f),			// direction
		glm::vec3(0.2f, 0.2f, 0.2f),		// ambient
		glm::vec3(0.5f, 0.5f, 0.5f),		// diffuse
		glm::vec3(1.0f, 1.0f, 1.0f),		// specular
		glm::cos(glm::radians(17.5f)),		// outer cutoff
		glm::cos(glm::radians(12.5f)),		// inner cutoff
		1.f,								// constant
		0.0014f,							// linear
		0.000007f							// quadratic
	};
	m_LightingManager->m_DirectionalLight = DirectionalLight{
		glm::vec3(-0.2f, -1.0f, -0.3f),		// direction
		glm::vec3(0.2f, 0.2f, 0.2f),		// ambient
		glm::vec3(0.5f, 0.5f, 0.5f),		// diffuse
		glm::vec3(1.0f, 1.0f, 1.0f),		// specular
	};
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

float Scene::GetSceneElapsedTime()
{
	return m_SceneElapsedTime;
}

void Scene::ClearScene()
{
	for (auto model : m_Models->m_Models)
	{
		model.second->DeleteModel();
	}

	m_Models->m_Models.clear();
	m_Primitives->m_Primitives.clear();
}

bool Scene::IsValid()
{
	return m_ValidScene;
}

void Scene::Render(float delta)
{
	UI::Instance().RenderUI(GetFrameBuffer()->Textures(), UI::CameraType::CAMERA_FLOATING);

	GetFrameBuffer()->Bind();

	Graphics::Instance()->Clear(glm::vec4(0.f, 0.f, 0.f, 1.f), true, false);

	m_SceneElapsedTime += delta;

	Graphics::Instance()->m_DrawCallsThisFrame = 0;
	Graphics::Instance()->m_TrianglesThisFrame = 0;

	GetPrimitiveContainer()->Render(delta);
	GetModelContainer()->Render(delta);

	GetFrameBuffer()->Unbind();
}
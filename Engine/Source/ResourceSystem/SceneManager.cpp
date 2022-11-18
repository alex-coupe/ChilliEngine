#include "SceneManager.h"
#include "../Rendering/Renderer.h"

namespace Chilli {
	
	void SceneManager::LoadScenes(const rapidjson::Value& scenes)
	{
		m_scenes.clear();
		m_currentScene.reset();
		DependencyResolver::ResolveDependency<Renderer>()->ClearRenderJobs();
		for (unsigned int i = 0; i < scenes.Size(); i++)
		{
			m_scenes.emplace_back(std::make_shared<Scene>(scenes[i]["SceneName"].GetString(), scenes[i]["Uuid"].GetUint64(), scenes[i]["Entities"].GetArray()));
		}
		m_currentScene = m_scenes.front();
	}

	std::string SceneManager::SaveScenes()const
	{
		std::stringstream ss;
		for (size_t i = 0; i < m_scenes.size(); i++)
		{
			ss << m_scenes[i]->Serialize();
			if (i != m_scenes.size() - 1)
				ss << ",";
		}
		return ss.str();
	}

	void SceneManager::Reset()
	{
		m_scenes.clear();
		m_currentScene = nullptr;
	}

	void SceneManager::AddScene(const std::string& name)
	{
		auto& scene = m_scenes.emplace_back(std::make_shared<Scene>(name));
		auto cameraEntity = std::make_shared<Entity>("Camera",0);
		cameraEntity->AddComponent(ComponentType::Camera);
		scene->AddEntity(cameraEntity);
	}

	void SceneManager::RemoveScene(UUID uuid)
	{
		if (auto m_sceneIterator = std::find_if(m_scenes.begin(), m_scenes.end(), [uuid](const std::shared_ptr<Scene> rhs)
			{
				return rhs->Uuid.Get() == uuid.Get();
			}); m_sceneIterator != m_scenes.end() && m_scenes.size() > 0)
		{
			for (const auto& ent : m_sceneIterator->get()->GetEntities())
				m_sceneIterator->get()->RemoveEntity(ent->Uuid);
			m_scenes.erase(m_sceneIterator);
		}
	}

	std::shared_ptr<Scene> SceneManager::GetSceneByUUID(UUID uuid)const
	{
		if (auto m_scenesItr = std::find_if(m_scenes.begin(), m_scenes.end(), [uuid](const std::shared_ptr<Scene> rhs)
			{
				return rhs->Uuid.Get() == uuid.Get();
			}); m_scenesItr != m_scenes.end() && m_scenes.size() > 0)
		{
			return *m_scenesItr;
		}
			return nullptr;
	}

	std::shared_ptr<Scene> SceneManager::GetSceneByName(const std::string& name)const
	{
		if (auto m_scenesItr = std::find_if(m_scenes.begin(), m_scenes.end(), [&name](const std::shared_ptr<Scene> rhs)
			{
				return rhs->GetName() == name;
			}); m_scenesItr != m_scenes.end() && m_scenes.size() > 0)
		{
			return *m_scenesItr;
		}
			return nullptr;
	}

	void SceneManager::GoToScene(UUID uuid)
	{
		auto scene = GetSceneByUUID(uuid);
		if (scene != nullptr)
			m_currentScene = scene;
	}

	void SceneManager::GoToScene(std::string& sceneName)
	{
		auto scene = GetSceneByName(sceneName);
		if (scene != nullptr)
			m_currentScene = scene;
	}

	void SceneManager::GoToPreviousScene()
	{
		if (auto m_scenesItr = std::find_if(m_scenes.begin(), m_scenes.end(), [this](const std::shared_ptr<Scene> rhs)
			{
				return rhs->GetName() == m_currentScene->GetName();
			}); m_scenesItr != m_scenes.end() && m_scenes.size() > 0 && m_scenesItr != m_scenes.begin())
		{
			m_currentScene = *(--m_scenesItr);
		}
	}

	void SceneManager::GoToNextScene()
	{
		if (auto m_scenesItr = std::find_if(m_scenes.begin(), m_scenes.end(), [this](const std::shared_ptr<Scene> rhs)
			{
				return rhs->GetName() == m_currentScene->GetName();
			}); m_scenesItr != m_scenes.end() && m_scenes.size() > 0)
		{	
				m_currentScene = *(++m_scenesItr);
		}
	}

	void SceneManager::StartCurrentScene()
	{
		CopyCurrentScene();
		m_currentScene->SetSceneState(SceneState::Play);
		m_currentScene->StartScene();
	}
	void SceneManager::StopCurrentScene()
	{
		m_currentScene->SetSceneState(SceneState::Edit);
		m_currentScene->StopScene();
		m_currentScene->Clone(m_currentSceneCopy);
		m_currentSceneCopy = nullptr;
	}

	bool SceneManager::HasScenes()const
	{
		return m_scenes.size() > 0 && m_currentScene;
	}

	void SceneManager::UpdateCurrentScene()const
	{
		if (m_currentScene->GetSceneState() == SceneState::Play ||
			m_currentScene->GetSceneState() == SceneState::Simulate) {
			m_currentScene->UpdateScene();
		}
	}

	const std::vector<std::shared_ptr<Scene>>& SceneManager::GetAllScenes()const
	{
		return m_scenes;
	}

	std::shared_ptr<Scene> SceneManager::GetCurrentScene()const
	{
		return m_currentScene;
	}

	void SceneManager::CopyCurrentScene()
	{
		m_currentSceneCopy = std::make_shared<Scene>(*m_currentScene);
	}
}
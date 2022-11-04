#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Scene.h"

namespace Chilli {

	class CHILLI_API SceneManager {
	public:
		void LoadScenes(const rapidjson::Value& scenes);
		std::string SaveScenes()const;
		void Reset();
		void AddScene(const std::string& name);
		void RemoveScene(UUID uuid);
		std::shared_ptr<Scene> GetSceneByUUID(UUID uuid)const;
		std::shared_ptr<Scene> GetCurrentScene()const;
		std::shared_ptr<Scene> GetSceneByName(const std::string& name)const;
		void GoToScene(UUID uuid);
		void GoToScene(std::string& sceneName);
		void GoToPreviousScene();
		void GoToNextScene();
		void StartCurrentScene();
		void StopCurrentScene();
		void UpdateCurrentScene()const;
		bool HasScenes()const;
		const std::vector<std::shared_ptr<Scene>>& GetAllScenes()const;
	private:
		void CopyCurrentScene();
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::shared_ptr<Scene> m_currentScene = nullptr;
		std::shared_ptr<Scene> m_currentSceneCopy = nullptr;
	};
}

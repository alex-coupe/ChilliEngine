#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Scene.h"

namespace Chilli {

	class CHILLI_API SceneManager {
	public:
		SceneManager();
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
		void StartCurrentScene()const;
		void StopCurrentScene()const;
		void UpdateCurrentScene()const;
		std::vector<std::shared_ptr<Script>> BuildAvailableScripts()const;
		const std::vector<std::shared_ptr<Scene>>& GetAllScenes()const;
	private:
		std::unique_ptr<ScriptEngine> m_scriptEngine = nullptr;
		std::shared_ptr<Scene> CopyCurrentScene()const;
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::shared_ptr<Scene> m_currentScene;
	};
}

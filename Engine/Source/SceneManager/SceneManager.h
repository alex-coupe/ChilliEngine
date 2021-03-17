#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Scene.h"

namespace Engine::SceneManager {

	class CHILLI_API SceneManager {
	public:
		SceneManager();
		void DrawGui()const;
		void LoadScenes(const std::string& filename);
		void SaveScenes(const std::string& filename);
		void AddScene();
		void RemoveScene(const char* scene_name);
		unsigned int GetCurrentScene()const;
		void SetCurrentScene(unsigned int scene_id);
		void DrawCurrentScene()const;
		void DrawGuiConst();
	private:
		std::shared_ptr<Scene> m_scenes;
		unsigned int m_currentSceneId;
	};
}
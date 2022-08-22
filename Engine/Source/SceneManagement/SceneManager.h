#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/SubSystem.h"
#include "../Core/DependencyResolver.h"
#include "../Core/Timer.h"
#include "Scene.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <fstream>
#include <sstream>

namespace Engine::SceneManagement {

	/// <summary>
	/// TODO - Mesh codex to avoid loading meshes twice
	/// </summary>

	class CHILLI_API SceneManager : public Engine::Core::SubSystem {
	public:
		SceneManager();
		void DrawGui();
		void LoadProject(const std::string& filename);
		void SaveProject(const std::string& filename);
		void AddScene(const std::string& name);
		void RemoveScene(unsigned int scene_id);
		unsigned int GetCurrentSceneId()const;
		std::shared_ptr<Scene> GetCurrentScene()const;
		void SetCurrentScene(unsigned int scene_id);
		void DrawCurrentScene()const;
		virtual int GetSystemType()const;
		virtual void ProcessFrame();
	private:
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::shared_ptr<Scene> m_currentScene;
		unsigned int m_currentSceneId = 1;
		unsigned int m_lastSceneId = 1;
		std::stringstream ss;
		std::string m_projectName = "untitled";
		std::shared_ptr<Engine::Core::Timer> m_timer;
	};
}
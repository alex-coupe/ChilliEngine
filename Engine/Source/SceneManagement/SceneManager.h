#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/SubSystem.h"
#include "../Core/DependencyResolver.h"
#include "Scene.h"
#include "MeshLibrary.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include <fstream>
#include <sstream>
namespace Engine::SceneManagement {

	class CHILLI_API SceneManager : public Engine::Core::SubSystem {
	public:
		SceneManager(const std::shared_ptr<Engine::Core::DependencyResolver<SubSystem>>& resolver);
		~SceneManager();
		void DrawGui()const;
		void LoadProject(const std::string& filename);
		void SaveProject(const std::string& filename);
		void AddScene(const std::string& name);
		void RemoveScene(unsigned int scene_id);
		unsigned int GetCurrentSceneId()const;
		std::shared_ptr<Scene> GetCurrentScene()const;
		void SetCurrentScene(unsigned int scene_id);
		void DrawCurrentScene()const;
		virtual int GetHash()const;
		virtual void ProcessFrame();
	private:
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::shared_ptr<Scene> m_currentScene;
		std::shared_ptr<MeshLibrary> m_meshLibrary;
		unsigned int m_currentSceneId = 0;
		std::stringstream ss;
		std::string m_projectName = "untitled";
	
	};
}
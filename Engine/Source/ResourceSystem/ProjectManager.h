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

namespace Engine::ResourceSystem {

	class CHILLI_API ProjectManager : public Engine::Core::SubSystem {
	public:
		ProjectManager();
		void LoadProject(const std::string& filename);
		void SaveProject(const std::string& filename);
		void NewProject();
		void AddScene(const std::string& name);
		void RemoveScene(Engine::Utilities::UUID& uuid);
		void AddAsset(const std::string& filename, const std::string& name, AssetTypes type);
		void RemoveAsset(Engine::Utilities::UUID& uuid);
		std::shared_ptr<Scene> GetCurrentScene()const;
		void SetCurrentScene(Engine::Utilities::UUID& uuid);
		virtual int GetSystemType()const;
		virtual void ProcessFrame();
	private:
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::vector<std::shared_ptr<Asset>> m_assets;
		std::shared_ptr<Scene> m_currentScene;
		std::stringstream ss;
		std::string m_projectName = "untitled project";
	};
}
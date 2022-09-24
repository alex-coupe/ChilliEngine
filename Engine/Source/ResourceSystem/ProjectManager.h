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
#include "../ECS/Component.h"

namespace Chilli {

	class CHILLI_API ProjectManager : public SubSystem {
	public:
		ProjectManager();
		void LoadProject(const std::string& filename);
		void SaveProject(const std::string& filename);
		void NewProject();
		void AddScene(const std::string& name);
		void RemoveScene(UUID uuid);
		std::vector<std::shared_ptr<Scene>> GetScenes()const;
		void AddAsset(const std::filesystem::path& filename, AssetTypes type);
		void RemoveAsset(UUID uuid);
		std::shared_ptr<Scene> GetCurrentScene()const;
		void SetCurrentScene(UUID uuid);
		void SetCurrentSceneState(SceneState state);
		std::shared_ptr<Asset> GetAssetByUUID(UUID uuid);
		std::vector<std::shared_ptr<Asset>> GetAssetsByType(AssetTypes type);
		const std::vector<std::string>& GetAvailableScripts()const;
		std::vector<std::shared_ptr<Component>> GetCurrentSceneComponentsByType(ComponentTypes type);
		static SystemType GetSystemType();
		virtual void ProcessFrame()override;
	private:
		std::unique_ptr<ScriptEngine> m_scriptEngine = nullptr;
		std::vector<std::shared_ptr<Scene>> m_scenes;
		std::shared_ptr<Scene> m_currentSceneCopy = nullptr;
		std::vector<std::shared_ptr<Asset>> m_assets;
		std::shared_ptr<Scene> m_currentScene;
		std::string m_projectName = "untitled project";
	};
}
#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/SubSystem.h"
#include "../Core/DependencyResolver.h"
#include "../Core/Timer.h"
#include <fstream>
#include <sstream>
#include "../ECS/Component.h"
#include "SceneManager.h"
#include "AssetManager.h"

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
		void AddAsset(const std::filesystem::path& filename, AssetType type);
		void AddScript(const std::string& className);
		void RemoveAsset(UUID uuid, AssetType type);
		std::shared_ptr<Scene> GetCurrentScene()const;
		void SetCurrentScene(UUID uuid);
		void PlayCurrentScene()const;
		void StopCurrentScene()const;
		std::shared_ptr<Asset> GetAssetByUUID(UUID uuid, AssetType type);
		const std::unordered_map<uint64_t, std::shared_ptr<Mesh>>& GetMeshes()const;
		static SystemType GetSystemType();
		virtual void ProcessFrame()override;
	private:
		std::unique_ptr<SceneManager> m_sceneManager = nullptr;
		std::unique_ptr<AssetManager> m_assetManager = nullptr;
		std::string m_projectName = "untitled project";
	};
}
#pragma once
#include "../Core/ChilliDefinitions.h"
#include <string>
#include "../ECS/Entity.h"
#include "../Utilities/UUID.h"

namespace Engine::ResourceSystem {

	enum class SceneState : int {
		Edit = 0, Play = 1, Pause = 2, Simulate = 3
	};

	using namespace Engine::ECS;

	class CHILLI_API Scene {
	public:
		Scene(const std::string& name);
		Scene(const std::string& name, Engine::Utilities::UUID uuid, const rapidjson::Value& entities);
		const std::string Serialize();
		void AddEntity(const std::string& name);
		void RemoveEntity(const Engine::Utilities::UUID& uuid);
		const std::vector<std::shared_ptr<Entity>>& GetEntities()const;
		void Update(float dt, bool isEditor);
		const Engine::Utilities::UUID& GetUUID()const;
		const std::string& GetName()const;
		const SceneState GetSceneState()const;
		void SetSceneState(SceneState state);
	private:
		std::string m_name;
		SceneState m_sceneState = SceneState::Edit;
		Engine::Utilities::UUID m_uuid;
		std::vector<std::shared_ptr<Entity>> m_entities;
	};
}
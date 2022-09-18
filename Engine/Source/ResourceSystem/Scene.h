#pragma once
#include "../Core/ChilliDefinitions.h"
#include <string>
#include "../ECS/Entity.h"
#include "../Utilities/UUID.h"
#include <memory>

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
		const Engine::Utilities::UUID& GetUUID()const;
		const std::string& GetName()const;
		const SceneState GetSceneState()const;
		void SetSceneState(SceneState state);
		void onSceneStart();
		void onSceneUpdate();
		void onSceneStop();
	private:
		std::vector<std::shared_ptr<Entity>> m_entitiesClone;
		std::string m_name;
		SceneState m_sceneState = SceneState::Edit;
		Engine::Utilities::UUID m_uuid;
		std::vector<std::shared_ptr<Entity>> m_entities;
		std::unique_ptr<b2World> m_b2World = nullptr;
		const float m_physicsTimestep = 1.0f / 60.0f;
		const int m_velocityIterations = 6;
		const int m_positionIterations = 2;
		const b2Vec2 m_gravity = { 0.0f, -1.0f };
	};
}
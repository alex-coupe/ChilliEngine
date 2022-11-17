#pragma once
#include "../Core/ChilliDefinitions.h"
#include <string>
#include "../ECS/Entity.h"
#include "../Core/UUID.h"
#include <memory>
#include "../Scripting/ScriptEngine.h"
#include "../Rendering/Camera.h"

namespace Chilli {

	enum class SceneState : int {
		Edit = 0, Play = 1, Pause = 2, Simulate = 3
	};

	class CHILLI_API Scene {
	public:
		Scene(const std::string& name);
		Scene(const std::string& name, UUID uuid, const rapidjson::Value& entities);
		Scene(const Scene& rhs);
		const std::string Serialize();
		void AddEntity(const std::string& name);
		void AddEntity(const std::shared_ptr<Entity> entity);
		void DuplicateEntity(const std::shared_ptr<Entity> entity);
		void RemoveEntity(UUID uuid);
		const std::vector<std::shared_ptr<Entity>>& GetEntities()const;
		std::shared_ptr<Entity> GetEntityByUUID(UUID uuid)const;
		std::shared_ptr<Entity> GetEntityByName(const std::string& name)const;
		const UUID Uuid;
		const std::string& GetName()const;
		void SetName(const std::string& name);
		const SceneState GetSceneState()const;
		void SetSceneState(SceneState state);
		void StartScene();
		void UpdateScene();
		void StopScene();
		void Clone(const std::shared_ptr<Scene>& scene);
	private:
		std::string m_name;
		std::unique_ptr<Camera> m_sceneCamera;
		SceneState m_sceneState = SceneState::Edit;
		std::vector<std::shared_ptr<Entity>> m_entities;
		std::unique_ptr<b2World> m_b2World = nullptr;
		const float m_physicsTimestep = 1.0f / 60.0f;
		const int m_velocityIterations = 6;
		const int m_positionIterations = 2;
		const b2Vec2 m_gravity = { 0.0f, -1.0f };
	};
}
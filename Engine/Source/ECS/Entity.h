#pragma once
#include "../Core/ChilliDefinitions.h"
#include <memory>
#include <vector>
#include "ComponentFactory.h"
#include "../Core/UUID.h"
#include "../Scripting/ScriptInstanceRepository.h"

namespace Chilli {

	class CHILLI_API Entity {
	public:
		Entity(const std::string& name);
		Entity(const std::string& name, UUID uuid, const rapidjson::Value& components);
		Entity(const Entity& rhs);
		void Clone(const Entity& rhs);
		std::shared_ptr<Component> GetComponentByType(ComponentType type);
		std::shared_ptr<Component> GetComponentByName(const char* name);
		void AddComponent(ComponentType type);
		const std::vector<std::shared_ptr<Component>>& GetComponents()const;
		void RemoveComponent(ComponentType type);
		bool HasComponent(ComponentType type);
		const std::string Serialize()const;
		const UUID Uuid;
		const std::string& GetName()const;
		const std::shared_ptr<TransformComponent> GetTransformComponent();
		void InitPhysics(std::unique_ptr<b2World>& physicsWorld);
		void UpdatePhysics();
		void AddCameraComponent();
	private:
		b2Body* CreateRigidBody(std::unique_ptr<b2World>& physicsWorld, const std::shared_ptr<TransformComponent> transform);
		void CreateBoxCollider(b2Body* rigidBody, const std::shared_ptr<TransformComponent> transform);
		void CreateCircleCollider(b2Body* rigidBody, const std::shared_ptr<TransformComponent> transform);
		std::string m_name;
		std::vector<std::shared_ptr<Component>> m_components;
		int64_t m_renderJobId = 0;
	};
}

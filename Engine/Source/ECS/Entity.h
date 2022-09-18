#pragma once
#include "../Core/ChilliDefinitions.h"
#include <memory>
#include <vector>
#include "ComponentFactory.h"
#include "../Utilities/UUID.h"

namespace Engine::ECS {

	class CHILLI_API Entity {
	public:
		Entity(const std::string& name);
		Entity(const std::string& name, Engine::Utilities::UUID uuid, const rapidjson::Value& components);
		std::shared_ptr<Component> GetComponentByType(ComponentTypes type);
		std::shared_ptr<Component> GetComponentByName(const char* name);
		void AddComponent(ComponentTypes type);
		const std::vector<std::shared_ptr<Component>>& GetComponents()const;
		void RemoveComponent(ComponentTypes type);
		bool HasComponent(ComponentTypes type);
		const std::string Serialize()const;
		const Engine::Utilities::UUID& GetUUID()const;
		const std::string& GetName()const;
		const std::shared_ptr<TransformComponent> GetTransformComponent();
		void InitPhysics(std::unique_ptr<b2World>& physicsWorld);
		void UpdatePhysics();
		static std::shared_ptr<Engine::ECS::Entity> Clone(Entity entity);
	private:
		b2Body* CreateRigidBody(std::unique_ptr<b2World>& physicsWorld, const std::shared_ptr<TransformComponent> transform);
		void CreateBoxCollider(b2Body* rigidBody, const std::shared_ptr<TransformComponent> transform);
		void CreateCircleCollider(b2Body* rigidBody, const std::shared_ptr<TransformComponent> transform);
		std::string m_name;
		Engine::Utilities::UUID m_uuid;
		std::vector<std::shared_ptr<Component>> m_components;
	};
}

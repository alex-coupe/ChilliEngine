#pragma once
#include "../Core/ChilliDefinitions.h"
#include <memory>
#include <vector>
#include "ComponentFactory.h"
#include "../Core/UUID.h"

namespace Chilli {

	class CHILLI_API Entity {
	public:
		Entity(const std::string& name);
		Entity(const std::string& name, UUID uuid, const rapidjson::Value& components);
		std::shared_ptr<Component> GetComponentByType(ComponentTypes type);
		std::shared_ptr<Component> GetComponentByName(const char* name);
		void AddComponent(ComponentTypes type);
		void AddComponent(std::shared_ptr<Component> component);
		const std::vector<std::shared_ptr<Component>>& GetComponents()const;
		void RemoveComponent(ComponentTypes type);
		bool HasComponent(ComponentTypes type);
		const std::string Serialize()const;
		const UUID Uuid;
		const std::string& GetName()const;
		const std::shared_ptr<TransformComponent> GetTransformComponent();
		void InitPhysics(std::unique_ptr<b2World>& physicsWorld);
		void UpdatePhysics();
		static std::shared_ptr<Entity> Clone(Entity& entity);
	private:
		b2Body* CreateRigidBody(std::unique_ptr<b2World>& physicsWorld, const std::shared_ptr<TransformComponent> transform);
		void CreateBoxCollider(b2Body* rigidBody, const std::shared_ptr<TransformComponent> transform);
		void CreateCircleCollider(b2Body* rigidBody, const std::shared_ptr<TransformComponent> transform);
		std::string m_name;
		std::vector<std::shared_ptr<Component>> m_components;
	};
}

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
		Entity(const std::string& name, UUID parent);
		Entity(UUID uuid, UUID parent, const rapidjson::Value& children, const rapidjson::Value& components);
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
		void SetParent(const UUID parentId);
		void AddChild(const UUID childId);
		void RemoveChild(const UUID childId);
		bool HasChildren()const;
		bool HasParent();
		const std::vector<std::shared_ptr<Entity>> GetChildren()const;
		const std::shared_ptr<Entity> GetParent()const;
		const UUID GetParentId()const;
	private:
		b2Body* CreateRigidBody(std::unique_ptr<b2World>& physicsWorld, const std::shared_ptr<TransformComponent> transform);
		void CreateBoxCollider(b2Body* rigidBody, const std::shared_ptr<TransformComponent> transform);
		void CreateCircleCollider(b2Body* rigidBody, const std::shared_ptr<TransformComponent> transform);
		std::vector<std::shared_ptr<Component>> m_components;
		UUID m_parent;
		std::vector<UUID> m_children;
		int64_t m_renderJobId = 0;
	};
}

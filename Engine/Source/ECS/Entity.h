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
		Entity(const Entity& rhs);
		std::shared_ptr<Component> GetComponentByType(ComponentTypes type);
		std::shared_ptr<Component> GetComponentByName(const char* name);
		void AddComponent(ComponentTypes type, ComponentVariables* vars = {});
		const std::vector<std::shared_ptr<Component>>& GetComponents()const;
		void RemoveComponent(ComponentTypes type);
		bool HasComponent(ComponentTypes type);
		const std::string Serialize()const;
		const Engine::Utilities::UUID& GetUUID()const;
		const std::string& GetName()const;
		const std::shared_ptr<TransformComponent>& GetTransformComponent();
	private:
		std::string m_name;
		Engine::Utilities::UUID m_uuid;
		std::vector<std::shared_ptr<Component>> m_components;
	};
}

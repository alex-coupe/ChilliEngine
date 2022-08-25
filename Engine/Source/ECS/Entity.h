#pragma once
#include "../Core/ChilliDefinitions.h"
#include <memory>
#include <set>
#include <vector>
#define UUID_SYSTEM_GENERATOR
#include "uuid.h"
#include "ComponentFactory.h"

namespace Engine::ECS {

	class CHILLI_API Entity {
	public:
		Entity(const std::string& name);
		Entity(const std::string& name, const rapidjson::Value& components);
		std::shared_ptr<Component> GetComponentByType(ComponentTypes type);
		std::shared_ptr<Component> GetComponentByName(const char* name);
		void AddComponent(ComponentTypes type);
		void RemoveComponent(ComponentTypes type);
		const std::string Serialize()const;
		uuids::uuid GetUUID()const;
	private:
		std::string m_name;
		uuids::uuid m_uuid;
		std::set<std::shared_ptr<Component>> m_components;
	};
}

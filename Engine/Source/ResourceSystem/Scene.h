#pragma once
#include "../Core/ChilliDefinitions.h"
#include <string>
#include "../ECS/Entity.h"
#include "../Utilities/UUID.h"

namespace Engine::ResourceSystem {

	using namespace Engine::ECS;

	class CHILLI_API Scene {
	public:
		Scene(const std::string& name);
		Scene(const std::string& name, Engine::Utilities::UUID uuid, const rapidjson::Value& entities);
		const std::string Serialize();
		void AddEntity(const std::string& name);
		void RemoveEntity(const Engine::Utilities::UUID& uuid);
		const std::vector<std::shared_ptr<Entity>>& GetEntities()const;
		void Update(float dt);
		const Engine::Utilities::UUID& GetUUID()const;
		const std::string& GetName()const;
	private:
		std::string m_name;
		Engine::Utilities::UUID m_uuid;
		std::vector<std::shared_ptr<Entity>> m_entities;
	};
}
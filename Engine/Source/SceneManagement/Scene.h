#pragma once
#include "../Core/ChilliDefinitions.h"
#include <string>
#include "../ECS/Entity.h"

namespace Engine::SceneManagement {

	using namespace Engine::ECS;

	class CHILLI_API Scene {
	public:
		Scene(unsigned int id, const std::string& name);
		Scene(unsigned int id, const std::string& name, unsigned int last_scene_id, const rapidjson::Value& entities);
		const std::string Serialize();
		void AddEntity(const std::string& name);
		void RemoveEntity(unsigned int id);
		const std::vector<std::shared_ptr<Entity>>& GetEntities()const;
		void Update(float dt);
		void DrawGui();
		unsigned int GetId()const;
		const std::string& GetName()const;
	private:
		std::string m_name;
		const unsigned int m_id;
		unsigned int m_lastEntityId = 1;
		std::vector<std::shared_ptr<Entity>> m_entities;
	};
}
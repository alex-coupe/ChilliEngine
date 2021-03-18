#pragma once
#include "../Core/ChilliDefinitions.h"
#include <string>
#include "../ECS/Entity.h"

namespace Engine::SceneManagement {

	using namespace Engine::ECS;

	class CHILLI_API Scene {
	public:
		Scene(unsigned int id, const std::string& name);
		Scene(unsigned int id, const std::string& name, const rapidjson::Value& entities);
		void Serialize(const std::string& filename);
		void Deserialize(const std::string& filename);
		void AddEntity();
		void RemoveEntity();
		const std::vector<std::shared_ptr<Entity>>& GetEntities()const;
		void Update(float dt);
		void DrawGui()const;
		void Draw()const;
		unsigned int GetID()const;
		const std::string& GetName()const;
	private:
		std::string m_name;
		const unsigned int m_id;
		std::vector<std::shared_ptr<Entity>> m_entities;
	};
}
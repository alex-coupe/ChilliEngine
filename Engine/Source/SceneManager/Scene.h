#pragma once
#include "../Core/ChilliDefinitions.h"
#include <string>
#include "../ECS/Entity.h"

namespace Engine::SceneManager {

	using namespace Engine::ECS;

	class CHILLI_API Scene {
	public:
		Scene();
		void Serialize(const std::string& filename);
		void Deserialize(const std::string& filename);
		void AddEntity();
		void RemoveEntity();
		void Update(float dt);
		void DrawGui()const;
		void Draw()const;
	private:
		mutable std::string m_name;
		const unsigned int m_id;
		std::unique_ptr<Entity> m_entities;
	};
}
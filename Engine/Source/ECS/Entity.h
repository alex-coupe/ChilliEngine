#pragma once
#include "../Core/ChilliDefinitions.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include <memory>
#include <set>
#include <vector>
/// <summary>
/// TODO - Entity factor to make prefabs such as point lights, actors, game objects etc that come with the relevent components
/// </summary>

namespace Engine::ECS {

	class CHILLI_API Entity {
	public:
		Entity(const std::string& name, const unsigned int id);
		Entity(const std::string& name, const unsigned int id, const rapidjson::Value& components);
		std::shared_ptr<Component> GetComponent(ComponentTypes type);
		void AddComponent(ComponentTypes type);
		void RemoveComponent(ComponentTypes type);
		const std::string Serialize()const;
		void DrawGui()const;
	private:
		std::string m_name;
		const unsigned int m_id;
		bool m_hasMesh = false;
		std::set<std::shared_ptr<Component>> m_components;
	};
}

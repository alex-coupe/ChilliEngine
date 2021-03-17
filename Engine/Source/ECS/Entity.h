#pragma once
#include "../Core/ChilliDefinitions.h"
#include "TransformComponent.h"
#include <memory>
#include <set>
#include <vector>

namespace Engine::ECS {

	class CHILLI_API Entity {
	public:
		Entity(const std::string& name, const unsigned int id);
		Entity(const std::string& name, const unsigned int id, const rapidjson::Value& components);
		std::shared_ptr<Component> GetComponent(const char* name);
		void AddComponent(const char* name);
		void RemoveComponent(const char* name);
		void Serialize(const std::string& filename)const;
		void Deserialize(const std::string& filename);
		void DrawGui()const;
		void Draw()const;
	private:
		std::string m_name;
		const unsigned int m_id;
		bool m_hasMesh = false;
		std::set<std::shared_ptr<Component>> m_components;
	};
}

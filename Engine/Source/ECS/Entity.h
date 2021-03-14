#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include <memory>
#include <vector>

namespace Engine::ECS {

	class CHILLI_API Entity {
	public:
		Entity(const std::string& name, const unsigned int id);
		Component* GetComponent(const char* name);
		void AddComponent(const char* name);
		void RemoveComponent(const char* name);
		void Serialize(const std::string& filename)const;
		void Deserialize(const std::string& filename);
		void DrawGui()const;
		void Draw()const;
	private:
		mutable std::string m_name;
		const unsigned int m_id;
		bool m_hasMesh = false;
		std::vector<std::unique_ptr<Component*>> m_components;
	};
}

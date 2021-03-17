#include "Entity.h"

Engine::ECS::Entity::Entity(const std::string& name, const unsigned int id)
	: m_name(name), m_id(id)
{
}

Engine::ECS::Entity::Entity(const std::string& name, const unsigned int id, const rapidjson::Value& components)
	: m_name(name), m_id(id)
{
	for (unsigned int i = 0; i < components.Size(); i++)
	{
		
		if (components[i]["Type"].GetInt() == static_cast<unsigned int>(ComponentTypes::Transform))
		{
			//m_components.emplace(std::make_shared<TransformComponent>( 0.0f,0.0f,0.0f ,  0.0f,0.0f,0.0f , 0.0f,0.0f,0.0f ));

		}
		
		
	}
}

std::shared_ptr<Engine::ECS::Component>  Engine::ECS::Entity::GetComponent(const char* name)
{
	return std::shared_ptr<Engine::ECS::Component>();
}

void Engine::ECS::Entity::AddComponent(const char* name)
{
}

void Engine::ECS::Entity::RemoveComponent(const char* name)
{
}

void Engine::ECS::Entity::Serialize(const std::string& filename) const
{
}

void Engine::ECS::Entity::Deserialize(const std::string& filename)
{
}

void Engine::ECS::Entity::DrawGui() const
{
}

void Engine::ECS::Entity::Draw() const
{
}

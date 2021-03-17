#include "Entity.h"

Engine::ECS::Entity::Entity(const std::string& name, const unsigned int id)
	: m_name(name), m_id(id)
{
}

std::shared_ptr<Engine::ECS::Component*>  Engine::ECS::Entity::GetComponent(const char* name)
{
	return std::shared_ptr<Engine::ECS::Component*>();
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

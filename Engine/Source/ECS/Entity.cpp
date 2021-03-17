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
		
		switch (components[i]["Type"].GetInt())
		{
			case (int)ComponentTypes::Transform:
			{
				DirectX::XMFLOAT3 translation = { components[i]["PosX"].GetFloat(),components[i]["PosY"].GetFloat(), components[i]["PosZ"].GetFloat() };
				DirectX::XMFLOAT3 rotation = { components[i]["RotX"].GetFloat(),components[i]["RotY"].GetFloat(), components[i]["RotZ"].GetFloat() };
				DirectX::XMFLOAT3 scale = { components[i]["ScaleX"].GetFloat(),components[i]["ScaleY"].GetFloat(), components[i]["ScaleZ"].GetFloat() };
				m_components.emplace(std::make_shared<TransformComponent>(translation, rotation, scale));
				break;
			}
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

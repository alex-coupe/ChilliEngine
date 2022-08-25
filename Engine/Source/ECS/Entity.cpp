#include "Entity.h"

Engine::ECS::Entity::Entity(const std::string& name)
	: m_name(name)
{
	m_uuid = uuids::uuid_system_generator{}();
}

Engine::ECS::Entity::Entity(const std::string& name, uuids::uuid uuid, const rapidjson::Value& components)
	: m_name(name), m_uuid(uuid)
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
				m_components.emplace(ComponentFactory::MakeTransformComponent(translation, rotation, scale));
				break;
			}
			case (int)ComponentTypes::Mesh:
			{
				m_components.emplace(std::make_shared<MeshComponent>(components[i]["MeshName"].GetString()));
				break;
			}
		}
	}
}

std::shared_ptr<Engine::ECS::Component>  Engine::ECS::Entity::GetComponentByType(ComponentTypes type)
{
	for (const auto& comp : m_components)
	{
		if (comp->GetComponentType() == type)
			return comp;
	}
	return nullptr;
}

void Engine::ECS::Entity::AddComponent(ComponentTypes type)
{
	if (auto contains = std::find_if(m_components.cbegin(), m_components.cend(), [type](const std::shared_ptr<Component> rhs) {
		return rhs->GetComponentType() == type;
		}); contains == m_components.cend())
	{
		switch (type)
		{
			case ComponentTypes::Transform:
				m_components.emplace(ComponentFactory::MakeTransformComponent({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }));
				break;
			case ComponentTypes::Mesh:
				m_components.emplace(ComponentFactory::MakeMeshComponent());
				break;
		}
	}
}

void Engine::ECS::Entity::RemoveComponent(ComponentTypes type)
{
	if (auto m_compoIterator = std::find_if(m_components.begin(), m_components.end(), [type](const std::shared_ptr<Component> rhs) 
		{
			return rhs->GetComponentType() == type;
		}); m_compoIterator != m_components.end())
	{
		m_components.erase(m_compoIterator);
	}
}

const std::string Engine::ECS::Entity::Serialize() const
{
	std::stringstream ss;
	ss << "\"Name\":\"" << m_name << "\", \"Uuid\":\"" << m_uuid << "\", \"Components\":[";
	for (const auto& compo : m_components)
	{
		ss << compo->Serialize();
	}
	ss << "]";
	return ss.str();
}

uuids::uuid Engine::ECS::Entity::GetUUID() const
{
	return m_uuid;
}

std::shared_ptr<Engine::ECS::Component> Engine::ECS::Entity::GetComponentByName(const char* name)
{
	for (const auto& comp : m_components)
	{
		if (comp->GetName() == name)
			return comp;
	}
	return nullptr;
}





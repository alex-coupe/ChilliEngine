#include "Entity.h"

Engine::ECS::Entity::Entity(const std::string& name)
	: m_name(name), m_uuid()
{
	m_components.emplace_back(ComponentFactory::MakeTransformComponent(m_uuid,{ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f }));
}

Engine::ECS::Entity::Entity(const std::string& name, Engine::Utilities::UUID uuid, const rapidjson::Value& components)
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
				m_components.emplace_back(ComponentFactory::MakeTransformComponent(m_uuid,translation, rotation, scale));
				break;
			}
			case (int)ComponentTypes::Mesh:
			{
				m_components.emplace_back(std::make_shared<MeshComponent>(components[i]["MeshUuid"].GetString(),m_uuid));
				break;
			}
			case (int)ComponentTypes::RigidBody2D:
			{
				m_components.emplace_back(std::make_shared<RigidBody2DComponent>(m_uuid,(BodyType)components[i]["BodyType"].GetInt(),GetTransformComponent(), (bool)components[i]["FixedRotation"].GetInt()));
				break;
			}
		}
	}
}

Engine::ECS::Entity::Entity(const Entity& rhs)
{
	for (const auto& component : rhs.GetComponents())
	{
		switch (component->GetComponentType())
		{
		case ComponentTypes::Mesh:
			m_components.emplace_back(std::make_shared<MeshComponent>(*(std::static_pointer_cast<MeshComponent>(component))));
			break;
		case ComponentTypes::RigidBody2D:
			m_components.emplace_back(std::make_shared<RigidBody2DComponent>(*(std::static_pointer_cast<RigidBody2DComponent>(component))));
			break;
		case ComponentTypes::Transform:
			m_components.emplace_back(std::make_shared<TransformComponent>(*(std::static_pointer_cast<TransformComponent>(component))));
			break;
		}
	}
}

const std::string& Engine::ECS::Entity::GetName()const
{
	return m_name;
}

const std::shared_ptr<Engine::ECS::TransformComponent>& Engine::ECS::Entity::GetTransformComponent()
{
	for (const auto& comp : m_components)
	{
		if (comp->GetComponentType() == ComponentTypes::Transform)
			return std::static_pointer_cast<TransformComponent>(comp);
	}
	return nullptr;
}

const std::vector<std::shared_ptr<Engine::ECS::Component>>& Engine::ECS::Entity::GetComponents()const
{
	return m_components;
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

bool Engine::ECS::Entity::HasComponent(ComponentTypes type)
{
	auto contains = std::find_if(m_components.cbegin(), m_components.cend(), [type](const std::shared_ptr<Component> rhs) {
		return rhs->GetComponentType() == type;
		});
	return contains != m_components.cend();
}

void Engine::ECS::Entity::AddComponent(ComponentTypes type, ComponentVariables* vars)
{
	if (auto contains = std::find_if(m_components.cbegin(), m_components.cend(), [type](const std::shared_ptr<Component> rhs) {
		return rhs->GetComponentType() == type;
		}); contains == m_components.cend())
	{
		switch (type)
		{
		case ComponentTypes::Mesh:
			m_components.emplace_back(ComponentFactory::MakeMeshComponent(m_uuid));
			break;
		case ComponentTypes::RigidBody2D:
		{
			const auto rb2dVars = (RigidBody2DOptions*)vars;
			const auto& transform = std::static_pointer_cast<TransformComponent>(GetComponentByName("Transform"));
			m_components.emplace_back(ComponentFactory::MakeRigidBody2DComponent(m_uuid, rb2dVars->type, transform, rb2dVars->fixedRotation));
		}
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
	ss << "{\"Name\":\"" << m_name << "\", \"Uuid\":\"" << m_uuid.GetUUID() << "\", \"Components\":[";
	for (size_t i = 0; i < m_components.size(); i++)
	{
		ss << m_components[i]->Serialize();
		if (i != m_components.size() - 1)
			ss << ",";
	}
	ss << "]}";
	return ss.str();
}

const Engine::Utilities::UUID& Engine::ECS::Entity::GetUUID() const
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







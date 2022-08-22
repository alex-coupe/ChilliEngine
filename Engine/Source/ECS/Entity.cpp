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
			case (int)ComponentTypes::Mesh:
			{
				m_components.emplace(std::make_shared<MeshComponent>(components[i]["EditorOnly"].GetBool(), components[i]["MeshName"].GetString(), components[i]["Filepath"].GetString()));
				m_hasMesh = true;
				break;
			}
		}
	}
}

std::shared_ptr<Engine::ECS::Component>  Engine::ECS::Entity::GetComponent(ComponentTypes type)
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
		//Replace this with a component factory
		switch (type)
		{
		case ComponentTypes::Transform:
		{
			DirectX::XMFLOAT3 posRot = { 0.0f,0.0f,0.0f };
			DirectX::XMFLOAT3 scale = { 1.0f,1.0f,1.0f };
			m_components.emplace(std::make_shared<TransformComponent>(posRot, posRot, scale));
		}
			break;
		case ComponentTypes::Mesh:
		{
			m_components.emplace(std::make_shared<MeshComponent>(false));
			m_hasMesh = false;
		}
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
	ss << "\"Components\":[";
	for (const auto& compo : m_components)
	{
		ss << compo->Serialize();
	}
	ss << "]";
	return ss.str();
}


void Engine::ECS::Entity::DrawGui() const
{
	ImGui::Begin(m_name.c_str());
	for (const auto& components : m_components)
	{
		components->DrawGui();
		ImGui::Separator();
	}
	
}

const unsigned int Engine::ECS::Entity::GetId() const
{
	return m_id;
}





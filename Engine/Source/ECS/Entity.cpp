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

std::shared_ptr<Engine::ECS::Component>  Engine::ECS::Entity::GetComponent(const std::string& name)
{
	for (const auto& comp : m_components)
	{
		if (comp->GetName() == name)
			return comp;
	}
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
	ImGui::Begin(m_name.c_str());
	for (const auto& components : m_components)
	{
		components->DrawGui();
		ImGui::Separator();
	}
	ImGui::End();
}

void Engine::ECS::Entity::Draw() const
{
}

#include "MeshComponent.h"

Engine::ECS::MeshComponent::MeshComponent(bool editor_only, const std::string& mesh_name)
	:m_editorOnly(editor_only), m_meshName(mesh_name), Component(ComponentTypes::Mesh)
{
}

const std::string Engine::ECS::MeshComponent::Serialize() const
{
	std::stringstream ss;
	ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"MeshName\":\"" << m_meshName <<"\"}";
	return  ss.str();
}

void Engine::ECS::MeshComponent::DrawGui() const
{
	ImGui::Text(m_meshName.c_str());
}

#include "MeshComponent.h"

Engine::ECS::MeshComponent::MeshComponent(bool editor_only, const std::string& mesh_name, const std::string& filepath)
	:m_editorOnly(editor_only), m_meshName(mesh_name), Component(ComponentTypes::Mesh, "Mesh")
{
	m_mesh = std::make_shared<Engine::SceneManagement::Mesh>(filepath);
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

const std::vector<Engine::Rendering::VertexPos>& Engine::ECS::MeshComponent::GetVertices() const
{
	return m_mesh->GetVertices();
}

const std::vector<unsigned short>& Engine::ECS::MeshComponent::GetIndices() const
{
	return m_mesh->GetIndices();
}

#include "MeshComponent.h"

Engine::ECS::MeshComponent::MeshComponent(const char* mesh_name)
	:m_meshName(mesh_name), Component(ComponentTypes::Mesh, "Mesh")
{
	//Link to mesh from asset manager
}

const std::string& Engine::ECS::MeshComponent::GetMeshName()const
{
	return m_meshName;
}

const std::string Engine::ECS::MeshComponent::Serialize() const
{
	std::stringstream ss;
	ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"MeshName\":\"" << m_meshName <<"\"}";
	return  ss.str();
}

const std::vector<Engine::Rendering::VertexPos>& Engine::ECS::MeshComponent::GetVertices() const
{
	return m_mesh->GetVertices();
}

const std::vector<unsigned short>& Engine::ECS::MeshComponent::GetIndices() const
{
	return m_mesh->GetIndices();
}

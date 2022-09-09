#include "MeshComponent.h"
#include "../ResourceSystem/ProjectManager.h"

Engine::ECS::MeshComponent::MeshComponent(Engine::Utilities::UUID uuid)
	: m_meshUuid(uuid),Component(ComponentTypes::Mesh, "Mesh")
{
	m_mesh = std::static_pointer_cast<Engine::ResourceSystem::Mesh>(Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->GetAssetByUUID(m_meshUuid));
}

const std::string Engine::ECS::MeshComponent::Serialize() const
{
	std::stringstream ss;
	ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"MeshUuid\":\"" << m_mesh->GetUUID().GetUUID() << "\"}";
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

const std::shared_ptr<Engine::ResourceSystem::Mesh>& Engine::ECS::MeshComponent::GetMesh() const
{
	return m_mesh;
}

void Engine::ECS::MeshComponent::SetMesh(std::shared_ptr<Engine::ResourceSystem::Mesh> mesh)
{
	m_mesh = mesh;
	m_meshUuid = mesh->GetUUID();
}

void Engine::ECS::MeshComponent::Update(float dt, bool isEditor)
{
	return;
}

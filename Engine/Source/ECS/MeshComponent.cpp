#include "MeshComponent.h"
#include "../ResourceSystem/ProjectManager.h"

Engine::ECS::MeshComponent::MeshComponent(const Engine::Utilities::UUID& uuid, const Engine::Utilities::UUID& entityuuid)
	: m_meshUuid(uuid),Component(ComponentTypes::Mesh, "Mesh", entityuuid)
{
	m_mesh = std::static_pointer_cast<Engine::ResourceSystem::Mesh>(Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->GetAssetByUUID(m_meshUuid));
}

Engine::ECS::MeshComponent::MeshComponent(const MeshComponent& rhs)
	:Component(rhs.m_type,rhs.m_name, rhs.m_owningEntityUuid)
{
	m_mesh = std::make_shared<Engine::ResourceSystem::Mesh>(*rhs.m_mesh);
	m_meshUuid = rhs.m_meshUuid;
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


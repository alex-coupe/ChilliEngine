#include "MeshComponent.h"
#include "../ResourceSystem/ProjectManager.h"

namespace Chilli {
	MeshComponent::MeshComponent()
		: Component(ComponentTypes::Mesh, "Mesh")
	{}

	MeshComponent::MeshComponent(UUID meshUuid)
		: Component(ComponentTypes::Mesh, "Mesh"), m_meshUuid(meshUuid)
	{}

	MeshComponent::MeshComponent(const MeshComponent& rhs)
		:Component(rhs.m_type, rhs.m_name)
	{
		m_meshUuid = rhs.m_meshUuid;
	}

	const std::string MeshComponent::Serialize() const
	{
		std::stringstream ss;
		ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"MeshUuid\":" << m_meshUuid.Get() << "}";
		return  ss.str();
	}

	const std::vector<VertexPos>& MeshComponent::GetVertices() const
	{
		return std::static_pointer_cast<Mesh>(DependencyResolver::ResolveDependency<ProjectManager>()->GetAssetByUUID(m_meshUuid, AssetType::Mesh))->GetVertices();
	}

	const std::vector<unsigned short>& MeshComponent::GetIndices() const
	{
		return std::static_pointer_cast<Mesh>(DependencyResolver::ResolveDependency<ProjectManager>()->GetAssetByUUID(m_meshUuid, AssetType::Mesh))->GetIndices();
	}

	const std::shared_ptr<Mesh> MeshComponent::GetMesh() const
	{
		return std::static_pointer_cast<Mesh>(DependencyResolver::ResolveDependency<ProjectManager>()->GetAssetByUUID(m_meshUuid, AssetType::Mesh));
	}

	void MeshComponent::SetMesh(UUID meshUuid)
	{
		m_meshUuid = meshUuid;
	}
}


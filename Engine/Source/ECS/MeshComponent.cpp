#include "MeshComponent.h"
#include "../ResourceSystem/ProjectManager.h"

namespace Chilli {
	MeshComponent::MeshComponent()
		: Component(ComponentTypes::Mesh, "Mesh")
	{}

	MeshComponent::MeshComponent(UUID meshUuid)
		: Component(ComponentTypes::Mesh, "Mesh")
	{
		m_mesh = std::static_pointer_cast<Mesh>(DependencyResolver::ResolveDependency<ProjectManager>()->GetAssetByUUID(meshUuid));
	}

	MeshComponent::MeshComponent(const MeshComponent& rhs)
		:Component(rhs.m_type, rhs.m_name)
	{
		m_mesh = rhs.m_mesh;
	}

	const std::string MeshComponent::Serialize() const
	{
		std::stringstream ss;
		ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"MeshUuid\":" << m_mesh->Uuid.Get() << "}";
		return  ss.str();
	}

	const std::vector<VertexPos>& MeshComponent::GetVertices() const
	{
		return m_mesh->GetVertices();
	}

	const std::vector<unsigned short>& MeshComponent::GetIndices() const
	{
		return m_mesh->GetIndices();
	}

	const std::shared_ptr<Mesh> MeshComponent::GetMesh() const
	{
		return m_mesh;
	}

	void MeshComponent::SetMesh(std::shared_ptr<Mesh> mesh)
	{
		m_mesh = mesh;
	}
}


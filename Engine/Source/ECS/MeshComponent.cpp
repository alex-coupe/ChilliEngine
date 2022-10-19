#include "MeshComponent.h"
#include "../ResourceSystem/ProjectManager.h"

namespace Chilli {
	MeshComponent::MeshComponent()
		: Component(ComponentType::Mesh, "Mesh"), m_color(1.0f, 1.0f, 1.0f, 1.0f), m_meshUuid(UUID(0))
	{}

	MeshComponent::MeshComponent(UUID meshUuid)
		: Component(ComponentType::Mesh, "Mesh"), m_meshUuid(meshUuid), m_color(1.0f, 1.0f, 1.0f, 1.0f)
	{}

	MeshComponent::MeshComponent(const MeshComponent& rhs)
		:Component(rhs.m_type, rhs.m_name)
	{
		m_color = rhs.m_color;
		m_meshUuid = rhs.m_meshUuid;
	}

	void MeshComponent::Clone(const std::shared_ptr<Component>& rhs)
	{
		const auto& comp = std::static_pointer_cast<MeshComponent>(rhs);
		m_color = comp->m_color;
		m_meshUuid = comp->m_meshUuid;
	}

	const std::string MeshComponent::Serialize(uint64_t entityId) const
	{
		std::stringstream ss;
		ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"MeshUuid\":" << m_meshUuid.Get() << "}";
		return  ss.str();
	}

	const std::vector<VertexPosTexNorm>& MeshComponent::GetVertices() const
	{
		return std::static_pointer_cast<Mesh>(DependencyResolver::ResolveDependency<ProjectManager>()->GetAssetByUUID(m_meshUuid, AssetType::Mesh))->GetVertices();
	}

	const std::vector<unsigned short>& MeshComponent::GetIndices() const
	{
		return std::static_pointer_cast<Mesh>(DependencyResolver::ResolveDependency<ProjectManager>()->GetAssetByUUID(m_meshUuid, AssetType::Mesh))->GetIndices();
	}

	const std::shared_ptr<Mesh> MeshComponent::GetMesh() const
	{
		const auto mesh = std::static_pointer_cast<Mesh>(DependencyResolver::ResolveDependency<ProjectManager>()->GetAssetByUUID(m_meshUuid, AssetType::Mesh));
		if (mesh != nullptr)
			return mesh;

		return nullptr;
	}

	const bool MeshComponent::HasMesh()const
	{
		const auto& mesh = std::static_pointer_cast<Mesh>(DependencyResolver::ResolveDependency<ProjectManager>()->GetAssetByUUID(m_meshUuid, AssetType::Mesh));
		return mesh != nullptr;
	}

	void MeshComponent::SetMesh(UUID meshUuid)
	{
		m_meshUuid = meshUuid;
	}

	DirectX::XMFLOAT4& MeshComponent::Color()
	{
		return m_color;
	}
}


#include "MeshComponent.h"
#include "../ResourceSystem/ProjectManager.h"

namespace Chilli {
	MeshComponent::MeshComponent()
		: Component(ComponentType::Mesh, "Mesh"), m_meshUuid(UUID(0))
	{
		material = {};
		material.color = { 1.0f,1.0f,1.0f,1.0f };
		material.textureUuid = 0;
	}

	MeshComponent::MeshComponent(UUID meshUuid)
		: Component(ComponentType::Mesh, "Mesh"), m_meshUuid(meshUuid)
	{
		material = {};
		material.color = { 1.0f,1.0f,1.0f,1.0f };
		material.textureUuid = 0;
	}

	MeshComponent::MeshComponent(const MeshComponent& rhs)
		:Component(rhs.m_type, rhs.m_name)
	{
		material = rhs.material;
		m_meshUuid = rhs.m_meshUuid;
	}

	void MeshComponent::Clone(const std::shared_ptr<Component>& rhs)
	{
		const auto& comp = std::static_pointer_cast<MeshComponent>(rhs);
		material = comp->material;
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

	const std::shared_ptr<Texture> MeshComponent::GetTexture() const
	{
		const auto tex = std::static_pointer_cast<Texture>(DependencyResolver::ResolveDependency<ProjectManager>()->GetAssetByUUID(material.textureUuid, AssetType::Texture));
		if (tex != nullptr)
			return tex;

		return nullptr;
	}

	const bool MeshComponent::HasMesh()const
	{
		const auto& mesh = std::static_pointer_cast<Mesh>(DependencyResolver::ResolveDependency<ProjectManager>()->GetAssetByUUID(m_meshUuid, AssetType::Mesh));
		return mesh != nullptr;
	}

	const bool MeshComponent::HasTexture()const
	{
		return material.textureUuid.Get() != 0;
	}

	void MeshComponent::SetMesh(UUID meshUuid)
	{
		m_meshUuid = meshUuid;
	}

	void MeshComponent::SetTexture(UUID texUuid)
	{
		material.textureUuid = texUuid;
	}
}


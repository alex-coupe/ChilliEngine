#include "MeshComponent.h"
#include "../ResourceSystem/ProjectManager.h"

namespace Chilli {
	MeshComponent::MeshComponent()
		: Component(ComponentType::Mesh, "Mesh"), m_meshUuid(UUID(0)),m_textureUuid(0)
	{
		material = {};
		material.ambient = { 1.0f, 0.5f, 0.31f };
		material.diffuse = { 1.0f, 0.5f, 0.31f };
		material.specular = { 0.5f, 0.5f, 0.5f };
		material.shininess = 32.0f;
	}

	MeshComponent::MeshComponent(UUID meshUuid)
		: Component(ComponentType::Mesh, "Mesh"), m_meshUuid(meshUuid), m_textureUuid(0)
	{
		material = {};
		material.ambient = { 1.0f, 0.5f, 0.31f };
		material.diffuse = { 1.0f, 0.5f, 0.31f };
		material.specular = { 0.5f, 0.5f, 0.5f };
		material.shininess = 32.0f;
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
		ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"MeshUuid\":" << m_meshUuid.Get()
		<< ", \"TextureUuid\":" << m_textureUuid.Get() 
		<< ", \"AmbR\":" << material.ambient.x << ", \"AmbG\":" << material.ambient.y << ", \"AmbB\":" << material.ambient.z
		<< ", \"DiffR\":" << material.diffuse.x << ", \"DiffG\":" << material.diffuse.y << ", \"DiffB\":" << material.diffuse.z
		<< ", \"SpecR\":" << material.specular.x << ", \"SpecG\":" << material.specular.y << ", \"SpecB\":" << material.specular.z
		<< ", \"Shininess\":" << material.shininess
		<< "}";
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
		const auto tex = std::static_pointer_cast<Texture>(DependencyResolver::ResolveDependency<ProjectManager>()->GetAssetByUUID(m_textureUuid, AssetType::Texture));
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
		return m_textureUuid.Get() != 0;
	}

	void MeshComponent::SetMesh(UUID meshUuid)
	{
		m_meshUuid = meshUuid;
	}

	void MeshComponent::SetTexture(UUID texUuid)
	{
		m_textureUuid = texUuid;
	}
}


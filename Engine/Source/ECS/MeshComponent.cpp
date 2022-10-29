#include "MeshComponent.h"
#include "../ResourceSystem/ProjectManager.h"

namespace Chilli {
	MeshComponent::MeshComponent()
		: Component(ComponentType::Mesh, "Mesh"), m_meshUuid(UUID(0)),m_textureUuid(0), m_specularMap(0)
	{
		material = {};
		material.diffuse = { 1.0f, 1.0f, 1.0f };
		material.specular = { 0.5f, 0.5f, 0.5f };
		material.shininess = 32.0f;
	}

	MeshComponent::MeshComponent(UUID meshUuid, UUID textureId, UUID specularId, 
		DirectX::XMFLOAT3 diffuse, DirectX::XMFLOAT3 specular, float shininess)
		: Component(ComponentType::Mesh, "Mesh"), m_meshUuid(meshUuid), m_textureUuid(textureId), m_specularMap(specularId)
	{
		material = {};
		material.diffuse = diffuse;
		material.specular = specular;
		material.shininess = shininess;
	}

	MeshComponent::MeshComponent(const MeshComponent& rhs)
		:Component(rhs.m_type, rhs.m_name)
	{
		material = rhs.material;
		m_textureUuid = rhs.m_textureUuid;
		m_specularMap = rhs.m_specularMap;
		m_meshUuid = rhs.m_meshUuid;
	}

	void MeshComponent::Clone(const std::shared_ptr<Component>& rhs)
	{
		const auto& comp = std::static_pointer_cast<MeshComponent>(rhs);
		material = comp->material;
		m_meshUuid = comp->m_meshUuid;
		m_textureUuid = comp->m_textureUuid;
		m_specularMap = comp->m_specularMap;
	}

	const std::string MeshComponent::Serialize(uint64_t entityId) const
	{
		std::stringstream ss;
		ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"MeshUuid\":" << m_meshUuid.Get()
		<< ", \"TextureUuid\":" << m_textureUuid.Get() << ", \"SpecularMapUuid\":" << m_specularMap.Get()
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

	const std::shared_ptr<Texture> MeshComponent::GetSpecularMap() const
	{
		const auto tex = std::static_pointer_cast<Texture>(DependencyResolver::ResolveDependency<ProjectManager>()->GetAssetByUUID(m_specularMap, AssetType::Texture));
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

	const bool MeshComponent::HasSpecularMap()const
	{
		return m_specularMap.Get() != 0;
	}

	void MeshComponent::SetMesh(UUID meshUuid)
	{
		m_meshUuid = meshUuid;
	}

	void MeshComponent::SetTexture(UUID texUuid)
	{
		m_textureUuid = texUuid;
	}

	void MeshComponent::SetSpecularMap(UUID specularUuid)
	{
		m_specularMap = specularUuid;
	}
}


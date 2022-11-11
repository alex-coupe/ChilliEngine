#include "MeshComponent.h"
#include "../ResourceSystem/ProjectManager.h"

namespace Chilli {
	MeshComponent::MeshComponent()
		: Component(ComponentType::Mesh, "Mesh"), meshUuid(UUID(0)), materialUuid(UUID(0))
	{}

	MeshComponent::MeshComponent(UUID meshUuid, UUID materialUuid)
		: Component(ComponentType::Mesh, "Mesh"), meshUuid(meshUuid), materialUuid(materialUuid)
	{}

	MeshComponent::MeshComponent(const MeshComponent& rhs)
		:Component(rhs.m_type, rhs.m_name)
	{
		materialUuid = rhs.materialUuid;
		meshUuid = rhs.meshUuid;
	}

	void MeshComponent::Clone(const std::shared_ptr<Component>& rhs)
	{
		const auto& comp = std::static_pointer_cast<MeshComponent>(rhs);
		materialUuid = comp->materialUuid;
		meshUuid = comp->meshUuid;
	}

	const std::string MeshComponent::Serialize(uint64_t entityId) const
	{
		std::stringstream ss;
		ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"MeshUuid\":" << meshUuid.Get()
			<< ", \"MaterialUuid\":" << materialUuid.Get() << "}";
		return  ss.str();
	}

	const std::vector<VertexPosTexNorm>& MeshComponent::GetVertices() const
	{
		return std::static_pointer_cast<Mesh>(DependencyResolver::ResolveDependency<ProjectManager>()->GetAssetByUUID(meshUuid, AssetType::Mesh))->GetVertices();
	}

	const std::vector<unsigned short>& MeshComponent::GetIndices() const
	{
		return std::static_pointer_cast<Mesh>(DependencyResolver::ResolveDependency<ProjectManager>()->GetAssetByUUID(meshUuid, AssetType::Mesh))->GetIndices();
	}

	const std::shared_ptr<Mesh> MeshComponent::GetMesh() const
	{
		const auto mesh = std::static_pointer_cast<Mesh>(DependencyResolver::ResolveDependency<ProjectManager>()->GetAssetByUUID(meshUuid, AssetType::Mesh));
		if (mesh != nullptr)
			return mesh;

		return nullptr;
	}

	const std::shared_ptr<Texture> MeshComponent::GetTexture() const
	{
		auto& material = DependencyResolver::ResolveDependency<ProjectManager>()->GetMaterial(materialUuid.Get());
		const auto tex = std::static_pointer_cast<Texture>(DependencyResolver::ResolveDependency<ProjectManager>()->GetAssetByUUID(material.DiffuseTexId, AssetType::Texture));
		if (tex != nullptr)
			return tex;

		return nullptr;
	}

	const std::shared_ptr<Texture> MeshComponent::GetSpecularMap() const
	{
		auto& material = DependencyResolver::ResolveDependency<ProjectManager>()->GetMaterial(materialUuid.Get());
		const auto tex = std::static_pointer_cast<Texture>(DependencyResolver::ResolveDependency<ProjectManager>()->GetAssetByUUID(material.SpecularTexId, AssetType::Texture));
		if (tex != nullptr)
			return tex;

		return nullptr;
	}

	const Material& MeshComponent::GetMaterial() const
	{
		return DependencyResolver::ResolveDependency<ProjectManager>()->GetMaterial(materialUuid.Get());
	}

	const bool MeshComponent::HasMesh()const
	{
		const auto& mesh = std::static_pointer_cast<Mesh>(DependencyResolver::ResolveDependency<ProjectManager>()->GetAssetByUUID(meshUuid, AssetType::Mesh));
		return mesh != nullptr;
	}

	const bool MeshComponent::HasTexture()const
	{
		return DependencyResolver::ResolveDependency<ProjectManager>()->GetMaterial(materialUuid.Get()).DiffuseTexId.Get() != 0;
	}

	const bool MeshComponent::HasSpecularMap()const
	{
		return DependencyResolver::ResolveDependency<ProjectManager>()->GetMaterial(materialUuid.Get()).SpecularTexId.Get() != 0;
	}

	const ObjectMaterial& MeshComponent::GetObjectMaterial()
	{
		auto& material = DependencyResolver::ResolveDependency<ProjectManager>()->GetMaterial(materialUuid.Get());
		m_objectMat.diffuse.x = material.DiffuseColor[0];
		m_objectMat.diffuse.y = material.DiffuseColor[1];
		m_objectMat.diffuse.z = material.DiffuseColor[2];

		m_objectMat.specular.x = material.SpecularColor[0];
		m_objectMat.specular.y = material.SpecularColor[1];
		m_objectMat.specular.z = material.SpecularColor[2];

		m_objectMat.shininess = material.Shininess;
		return m_objectMat;
	}

	void MeshComponent::SetMesh(UUID Uuid)
	{
		meshUuid = Uuid;
	}
}


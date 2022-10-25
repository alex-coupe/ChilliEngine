#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include "../ResourceSystem/Mesh.h"
#include "../Rendering/Texture.h"

namespace Chilli {

	struct Material {
		alignas(16)DirectX::XMFLOAT3 diffuse;
		alignas(16)DirectX::XMFLOAT3 specular;
		float shininess;
	};

	class CHILLI_API MeshComponent : public Component {
	public:
		MeshComponent();
		MeshComponent(UUID meshUuid, UUID textureUuid,UUID specularUuid,DirectX::XMFLOAT3 diffuse, DirectX::XMFLOAT3 specular, float shininess);
		MeshComponent(const MeshComponent& rhs);
		virtual const std::string Serialize(uint64_t entityId)const override;
		virtual void Clone(const std::shared_ptr<Component>& rhs)override;
		const std::vector<VertexPosTexNorm>& GetVertices()const;
		const std::vector<unsigned short>& GetIndices()const;
		const std::shared_ptr<Mesh> GetMesh()const;
		const std::shared_ptr<Texture> GetTexture()const;
		const std::shared_ptr<Texture> GetSpecularMap()const;
		void SetMesh(UUID meshUuid);
		void SetTexture(UUID textureUuid);
		void SetSpecularMap(UUID specularUuid);
		const bool HasMesh()const;
		const bool HasTexture()const;
		const bool HasSpecularMap()const;
		Material material;
	private:
		UUID m_meshUuid;
		UUID m_textureUuid;
		UUID m_specularMap;
	};
}
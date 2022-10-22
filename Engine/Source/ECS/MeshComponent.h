#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include "../ResourceSystem/Mesh.h"
#include "../Rendering/Texture.h"

namespace Chilli {

	struct Material {
		alignas(16)DirectX::XMFLOAT3 ambient;
		alignas(16)DirectX::XMFLOAT3 diffuse;
		alignas(16)DirectX::XMFLOAT3 specular;
		float shininess;
	};

	class CHILLI_API MeshComponent : public Component {
	public:
		MeshComponent();
		MeshComponent(UUID meshUuid);
		MeshComponent(const MeshComponent& rhs);
		virtual const std::string Serialize(uint64_t entityId)const override;
		virtual void Clone(const std::shared_ptr<Component>& rhs)override;
		const std::vector<VertexPosTexNorm>& GetVertices()const;
		const std::vector<unsigned short>& GetIndices()const;
		const std::shared_ptr<Mesh> GetMesh()const;
		const std::shared_ptr<Texture> GetTexture()const;
		void SetMesh(UUID meshUuid);
		void SetTexture(UUID textureUuid);
		const bool HasMesh()const;
		const bool HasTexture()const;
		Material material;
	private:
		UUID m_meshUuid;
		UUID m_textureUuid;
	};
}
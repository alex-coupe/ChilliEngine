#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include "../ResourceSystem/Mesh.h"

namespace Chilli {

	struct Material {
		DirectX::XMFLOAT4 color;
		UUID textureUuid;
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
		void SetMesh(UUID meshUuid);
		void SetTexture(UUID textureUuid);
		const bool HasMesh()const;
		Material material;
	private:
		UUID m_meshUuid;
	};
}
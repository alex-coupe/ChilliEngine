#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include "../ResourceSystem/Mesh.h"
#include "../Rendering/Texture.h"
#include "../ResourceSystem/Material.h"

namespace Chilli {

	struct ObjectMaterial {
		alignas(16)DirectX::XMFLOAT3 diffuse;
		alignas(16)DirectX::XMFLOAT3 specular;
		float shininess;
	};

	class CHILLI_API MeshComponent : public Component {
	public:
		MeshComponent();
		MeshComponent(UUID meshUuid,UUID materialUuid);
		MeshComponent(const MeshComponent& rhs);
		virtual const std::string Serialize(uint64_t entityId)const override;
		virtual void Clone(const std::shared_ptr<Component>& rhs)override;
		const std::vector<VertexPosTexNorm>& GetVertices()const;
		const std::vector<unsigned short>& GetIndices()const;
		const std::shared_ptr<Mesh> GetMesh()const;
		const std::shared_ptr<Texture> GetTexture()const;
		const std::shared_ptr<Texture> GetSpecularMap()const;
		const Material& GetMaterial()const;
		void SetMesh(UUID meshUuid);
		const bool HasMesh()const;
		const bool HasTexture()const;
		const bool HasSpecularMap()const;
		const ObjectMaterial& GetObjectMaterial();
		UUID materialUuid;
		UUID meshUuid;
	private:
		ObjectMaterial m_objectMat;
	};
}
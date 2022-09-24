#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include "../ResourceSystem/Mesh.h"

namespace Chilli {

	class CHILLI_API MeshComponent : public Component {
	public:
		MeshComponent();
		MeshComponent(UUID meshUuid);
		MeshComponent(const MeshComponent& rhs);
		virtual const std::string Serialize()const override;
		const std::vector<VertexPos>& GetVertices()const;
		const std::vector<unsigned short>& GetIndices()const;
		const std::shared_ptr<Mesh> GetMesh()const;
		void SetMesh(std::shared_ptr<Mesh> mesh);
	private:
		std::shared_ptr<Mesh> m_mesh = nullptr;
	};
}
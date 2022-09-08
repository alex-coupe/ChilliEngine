#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include "../ResourceSystem/Mesh.h"

namespace Engine::ECS {

	class CHILLI_API MeshComponent : public Component {
	public:
		MeshComponent(Engine::Utilities::UUID uuid);
		virtual const std::string Serialize()const override;
		const std::vector<Engine::Rendering::VertexPos>& GetVertices()const;
		const std::vector<unsigned short>& GetIndices()const;
		const std::shared_ptr<Engine::ResourceSystem::Mesh>& GetMesh()const;
		void SetMesh(std::shared_ptr<Engine::ResourceSystem::Mesh> mesh);
	private:
		std::shared_ptr<Engine::ResourceSystem::Mesh> m_mesh;
		Engine::Utilities::UUID m_meshUuid;
	};
}
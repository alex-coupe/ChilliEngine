#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include "../SceneManagement/Mesh.h"

namespace Engine::ECS {

	class CHILLI_API MeshComponent : public Component {
	public:
		MeshComponent(const char* mesh_name);
		virtual const std::string Serialize()const override;
		const std::vector<Engine::Rendering::VertexPos>& GetVertices()const;
		const std::vector<unsigned short>& GetIndices()const;
		const std::string& GetMeshName()const;
	private:
		std::string m_meshName;
		std::shared_ptr<Engine::SceneManagement::Mesh> m_mesh;
	};
}
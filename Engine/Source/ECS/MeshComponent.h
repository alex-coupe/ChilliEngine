#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include "../SceneManagement/Mesh.h"

namespace Engine::ECS {

	class CHILLI_API MeshComponent : public Component {
	public:
		MeshComponent(bool editor_only, const std::string& mesh_name, const std::string& filepath);
		MeshComponent(bool editor_only);
		virtual const std::string Serialize()const override;
		virtual void DrawGui()const override;
		const std::vector<Engine::Rendering::VertexPos>& GetVertices()const;
		const std::vector<unsigned short>& GetIndices()const;
	private:
		bool m_editorOnly;
		std::string m_meshName;
		std::shared_ptr<Engine::SceneManagement::Mesh> m_mesh;
	};
}
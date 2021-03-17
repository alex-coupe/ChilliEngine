#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"

namespace Engine::ECS {

	class CHILLI_API MeshComponent : public Component {
	public:
		MeshComponent();
		virtual void Serialize(const std::string& filename)override;
		virtual void Deserialize(const std::string& filename)override;
		virtual void DrawGui()const override;
		void Draw()const;
	private:
		bool m_editorOnly = false;
	};
}
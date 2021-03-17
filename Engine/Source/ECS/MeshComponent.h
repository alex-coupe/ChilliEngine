#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"

namespace Engine::ECS {

	class CHILLI_API MeshComponent : public Component {
	public:
		MeshComponent();
		virtual const std::string Serialize()const override;
		virtual void DrawGui()const override;
	private:
		bool m_editorOnly = false;
	};
}
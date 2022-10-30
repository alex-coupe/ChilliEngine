#pragma once
#include "Core/ChilliDefinitions.h"
#include "Rendering/Renderer.h"
#include "ResourceSystem/ProjectManager.h"

namespace Chilli {

	class EditorPanel {
	public:
		EditorPanel(const std::string& panelName)
			: m_panelName(panelName)
		{}
		virtual void Draw() = 0;
	protected:
		std::string m_panelName;
	};
}
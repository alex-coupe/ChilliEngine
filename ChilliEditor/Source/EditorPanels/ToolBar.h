#pragma once
#include "Core/ChilliDefinitions.h"
#include "Rendering/Renderer.h"
#include "ResourceSystem/ProjectManager.h"

namespace Chilli {
	class ToolBar {
	public:
		ToolBar();
		void DrawGui()const;
	private:
		std::shared_ptr<Texture> m_playIcon;
		std::shared_ptr<Texture> m_stopIcon;
		std::shared_ptr<Texture> m_simulateIcon;
	};
}
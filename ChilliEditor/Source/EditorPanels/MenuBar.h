#pragma once
#include "Core/ChilliDefinitions.h"
#include "Rendering/Renderer.h"
#include "ResourceSystem/ProjectManager.h"

namespace Chilli {
	class MenuBar {
	public:
		void DrawGui();
	private:
		std::string m_savePath = "";
		void NewProject();
		void SaveProject();
		void OpenProject();
		void SaveAs();
	};
}
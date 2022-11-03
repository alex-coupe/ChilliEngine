#pragma once
#include "Core/ChilliDefinitions.h"
#include "Rendering/Renderer.h"
#include "ResourceSystem/ProjectManager.h"
#include "Core/Window.h"

namespace Chilli {
	class MenuBar {
	public:
		MenuBar(std::unique_ptr<Window>& window);
		void DrawGui();
	private:
		std::unique_ptr<Window>& m_window;
		void NewProject();
		void SaveProject();
		void OpenProject();
		void SaveAs();
	};
}
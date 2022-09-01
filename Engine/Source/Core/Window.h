#pragma once
#include "ChilliDefinitions.h"
#include "Events.h"
#include "SubSystem.h"
#include "DependencyResolver.h"
#include "../Gui/GuiManager.h"

#ifdef _WIN32
#include "Platform/WindowsPlatform.h"
#endif

namespace Engine::Core {
	class Window {
	public:
		Window(std::shared_ptr<Gui::GuiManager>& gui_man);
		bool Update();
		void* GetWindowHandle();
		~Window();
		const int GetWidth()const;
		const int GetHeight()const;
	private:
		int m_width = 0;
		int m_height = 0;
		std::shared_ptr<Gui::GuiManager>& m_gui;
#ifdef _WIN32
		std::unique_ptr<Platform::WindowsPlatform> m_platform;
#endif
	};
}


#pragma once
#include "ChilliDefinitions.h"
#include "Events.h"
#include "SubSystem.h"
#include "DependencyResolver.h"
#include "../Gui/GuiManager.h"

#ifdef _WIN32
#include "Platform/WindowsPlatform.h"
#endif

namespace Chilli {
	class Window {
	public:
		Window();
		bool Update();
		void* GetWindowHandle();
		~Window();
		const int GetWidth()const;
		const int GetHeight()const;
		void SetTitle(const char* title);
	private:
		int m_width = 0;
		int m_height = 0;
#ifdef _WIN32
		std::unique_ptr<WindowsPlatform> m_platform;
#endif
	};
}


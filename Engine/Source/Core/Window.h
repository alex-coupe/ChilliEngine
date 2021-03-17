#pragma once
#include "ChilliDefinitions.h"
#include <Windows.h>
#include "Event.h"
#include "SubSystem.h"

#include "../Gui/GuiManager.h"

namespace Engine::Core {
	class Window {
	public:
		Window(HINSTANCE& instance, const std::shared_ptr<Event>& event_in,std::shared_ptr<Gui::GuiManager>& gui_man);
		static LRESULT CALLBACK WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		HWND GetHandle();
		bool Update();
		~Window();
		const int GetInitialWidth()const;
		const int GetInitialHeight()const;
	private:
		HINSTANCE& m_instance;
		HWND m_handle = nullptr;
		int m_initialWidth = 0;
		int m_initialHeight = 0;
		static Window* m_pInstance;
		LRESULT CALLBACK MyWinProc(HWND, UINT, WPARAM, LPARAM);
		static constexpr wchar_t title[] = L"Chilli Engine";
		static constexpr wchar_t m_className[] = L"WindowClass";
		std::shared_ptr<Event> m_event;
		std::shared_ptr<Gui::GuiManager>& m_gui;
	};
}


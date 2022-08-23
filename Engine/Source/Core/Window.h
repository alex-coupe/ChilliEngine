#pragma once
#include "ChilliDefinitions.h"
#include <Windows.h>
#include "Events.h"
#include "SubSystem.h"
#include "DependencyResolver.h"
#include "../Gui/GuiManager.h"

namespace Engine::Core {
	class Window {
	public:
		Window(std::shared_ptr<Gui::GuiManager>& gui_man);
		static LRESULT CALLBACK WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		HWND GetHandle();
		bool Update();
		~Window();
		const int GetWidth()const;
		const int GetHeight()const;
	private:
		HWND m_handle = nullptr;
		int m_width = 0;
		int m_height = 0;
		static Window* m_pInstance;
		LRESULT CALLBACK MyWinProc(HWND, UINT, WPARAM, LPARAM);
		static constexpr wchar_t title[] = L"Chilli Engine";
		static constexpr wchar_t m_className[] = L"WindowClass";
		std::shared_ptr<Gui::GuiManager>& m_gui;
	};
}


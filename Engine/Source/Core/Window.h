#pragma once
#include "ChilliDefinitions.h"
#include <Windows.h>
#include "EventSystem.h"
#include "Logger.h"
#include "EngineSystem.h"
#include "../ImGui/imgui_impl_win32.h"
#include "../ImGui/imgui.h"

namespace Engine::Core {
	class Window {
	public:
		Window(HINSTANCE& instance, std::shared_ptr<EventSystem> event_system, bool fullscreen);
		static LRESULT CALLBACK WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		HWND GetHandle();
		bool Update();
		~Window();
		const int GetInitialWidth()const;
		const int GetInitialHeight()const;
	private:
		HINSTANCE& m_instance;
		HWND m_handle = nullptr;
		int m_initialWidth;
		int m_initialHeight;
		static Window* m_pInstance;
		LRESULT CALLBACK MyWinProc(HWND, UINT, WPARAM, LPARAM);
		static constexpr wchar_t title[] = L"Chilli Engine";
		static constexpr wchar_t m_className[] = L"WindowClass";
		std::shared_ptr<EventSystem> m_eventSystem = nullptr;
	};
}


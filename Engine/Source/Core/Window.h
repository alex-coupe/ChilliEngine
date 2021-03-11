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
		Window(HINSTANCE& instance, std::shared_ptr<EventSystem> event_system, int64_t width, int64_t height);
		static LRESULT CALLBACK WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		HWND GetHandle();
		int64_t GetWidth()const;
		int64_t GetHeight()const;
		bool Update();
		void DisplayWindow();
		~Window();
	private:
		
		HINSTANCE& m_instance;
		HWND m_handle = nullptr;
		int64_t m_width;
		int64_t m_height;
		static Window* m_pInstance;
		LRESULT CALLBACK MyWinProc(HWND, UINT, WPARAM, LPARAM);
		static constexpr wchar_t title[] = L"Chilli Engine";
		static constexpr wchar_t m_className[] = L"WindowClass";
		std::shared_ptr<EventSystem> m_eventSystem = nullptr;
	};
}


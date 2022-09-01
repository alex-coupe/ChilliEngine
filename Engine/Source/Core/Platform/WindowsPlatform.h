#pragma once
#include <Windows.h>
#include "../ChilliDefinitions.h"
#include "../../Gui/GuiManager.h"
#include "../../Core/Events.h"
#include "../DependencyResolver.h"

namespace Engine::Core::Platform {

	class WindowsPlatform {
	public:
		WindowsPlatform(std::shared_ptr<Engine::Gui::GuiManager>& gui_man, int& width, int& height);
		static LRESULT CALLBACK WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		HWND GetHandle();
		~WindowsPlatform();
		bool Update();
	private:
		HWND m_handle = nullptr;
		static WindowsPlatform* m_pInstance;
		LRESULT CALLBACK MyWinProc(HWND, UINT, WPARAM, LPARAM);
		static constexpr wchar_t title[] = L"Chilli Engine";
		static constexpr wchar_t m_className[] = L"WindowClass";
		std::shared_ptr<Gui::GuiManager>& m_gui;
	};
}
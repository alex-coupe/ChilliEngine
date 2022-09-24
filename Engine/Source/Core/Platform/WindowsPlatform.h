#pragma once
#include <Windows.h>
#include "../ChilliDefinitions.h"
#include "../../Gui/GuiManager.h"
#include "../../Core/Events.h"
#include "../DependencyResolver.h"

namespace Chilli {
	class CHILLI_API WindowsPlatform {
	public:
		WindowsPlatform(int& width, int& height);
		static LRESULT CALLBACK WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		HWND GetHandle();
		~WindowsPlatform();
		bool Update();
		void SetTitle(const char* title);
	private:
		HWND m_handle = nullptr;
		static WindowsPlatform* m_pInstance;
		LRESULT CALLBACK MyWinProc(HWND, UINT, WPARAM, LPARAM);
		static constexpr wchar_t title[] = L"Chilli Engine | Untitled Project";
		static constexpr wchar_t m_className[] = L"WindowClass";
	};
}
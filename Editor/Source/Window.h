#pragma once
#include <Windows.h>
#include "Core\ChilliEngine.h"

class Window {
public:
	Window(HINSTANCE& instance);
	static LRESULT CALLBACK WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
	const HWND& GetHandle()const;
	~Window();
private:
	HINSTANCE m_instance;
	HWND m_handle;
	static constexpr wchar_t title[] = L"Chilli Engine";
	static constexpr wchar_t m_className[] = L"WindowClass";
};


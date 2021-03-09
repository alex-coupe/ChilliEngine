#pragma once
#include <Windows.h>
#include "Core\ChilliEngine.h"
#include "Editor.h"


class Window {
public:
	Window(HINSTANCE& instance, std::shared_ptr<Editor> editor);
	static LRESULT CALLBACK WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
	HWND& GetHandle();
	bool Update();
	void DisplayWindow();
	~Window();
private:
	HINSTANCE m_instance;
	std::shared_ptr<Editor> m_editor = nullptr;
	HWND m_handle;
	static Window* m_pInstance;
	LRESULT CALLBACK MyWinProc(HWND, UINT, WPARAM, LPARAM);
	static constexpr wchar_t title[] = L"Chilli Engine";
	static constexpr wchar_t m_className[] = L"WindowClass";
};


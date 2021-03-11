#pragma once
#include <Windows.h>
#include "Core\ChilliEngine.h"

class Window {
public:
	Window(HINSTANCE& instance, unsigned int width, unsigned int height, std::shared_ptr<ChilliEngine> engine);
	static LRESULT CALLBACK WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
	HWND GetHandle();
	unsigned int GetWidth()const;
	unsigned int GetHeight()const;
	bool Update();
	~Window();
private:
		
	HINSTANCE& m_instance;
	HWND m_handle = nullptr;
	unsigned int m_width;
	unsigned int m_height;
	static Window* m_pInstance;
	LRESULT CALLBACK MyWinProc(HWND, UINT, WPARAM, LPARAM);
	static constexpr wchar_t title[] = L"Chilli Engine";
	static constexpr wchar_t m_className[] = L"WindowClass";
	std::shared_ptr<ChilliEngine> m_engine;
};



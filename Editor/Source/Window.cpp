#include "Window.h"

Window::Window(HINSTANCE& instance)
	:m_instance(instance), m_handle(nullptr)
{
	
	WNDCLASSEX wndclass = {};
	
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = 0;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = m_instance;
	wndclass.lpszClassName = m_className;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hCursor = LoadCursor(m_instance, IDC_ARROW);
	wndclass.hIcon = LoadIcon(instance, IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(instance, IDI_APPLICATION);
	wndclass.lpszMenuName = nullptr;
	
	if (!RegisterClassEx(&wndclass))
		EDITOR_ERROR("Failed To Register Window");

	EDITOR_INFO("Window Registered Successfully");

	m_handle = CreateWindowEx(
		0,
		m_className,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,NULL,NULL,m_instance,NULL);


	if (!m_handle)
	{
		EDITOR_ERROR("Failed To Create Window");
	}
	else
	{
		EDITOR_INFO("Window Created Successfully");
		ShowWindow(m_handle, SW_MAXIMIZE);
	}
	
}

LRESULT Window::WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(handle, msg, wParam, lParam);
}

const HWND& Window::GetHandle() const
{
	return m_handle;
}

Window::~Window()
{
	UnregisterClass(m_className,m_instance);
	EDITOR_INFO("Unregistering Window");
	DestroyWindow(m_handle);
	EDITOR_INFO("Destroying Window");
}

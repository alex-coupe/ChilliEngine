#include "Window.h"
#include "ImGui\imgui_impl_win32.h"
#include "ImGui\imgui.h"
#include "Editor.h"

Window* Window::m_pInstance = nullptr;

Window::Window(HINSTANCE& instance, std::shared_ptr<Editor> editor)
	:m_instance(instance), m_handle(nullptr), m_editor(editor)
{
	m_pInstance = this;
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
		WS_EX_CLIENTEDGE,
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
		
		//ImGui_ImplWin32_Init(m_handle);
	}
	
}

//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Window::WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	return m_pInstance->MyWinProc(handle, msg, wParam, lParam);
}

 HWND& Window::GetHandle() 
{
	return m_handle;
}

bool Window::Update()
{
	MSG msg = {};
	while (PeekMessage(&msg, m_handle, 0, 0, PM_REMOVE)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
			return false;
	}
	return true;
}

void Window::DisplayWindow()
{
	ShowWindow(m_handle, SW_MAXIMIZE);
}

Window::~Window()
{
	//ImGui_ImplWin32_Shutdown();
	UnregisterClass(m_className,m_instance);
	EDITOR_INFO("Unregistering Window");
	DestroyWindow(m_handle);
	EDITOR_INFO("Destroying Window");
}

LRESULT Window::MyWinProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	/*if (ImGui_ImplWin32_WndProcHandler(handle, msg, wParam, lParam))
{
	return true;
}
const auto& imio = ImGui::GetIO();
*/

	Engine::Core::EventData data = {};
	data.msg = msg;
	data.lparam = lParam;
	data.wparam = wParam;
	data.handle = &m_handle;

	//Raise Event For The Messages We're Interested In
	switch (msg)
	{
	case WM_SETFOCUS:
	case WM_KILLFOCUS:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	case WM_CHAR:
	case WM_MOVE:
	case WM_MOUSELEAVE:
		m_editor->RaiseEvent(data);
		break;
	}
		
	return DefWindowProc(handle, msg, wParam, lParam);
}

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
		//ImGui_ImplWin32_Init(m_handle);
	}
	
}

//extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Window::WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	return m_pInstance->MyWinProc(handle, msg, wParam, lParam);
}

const HWND& Window::GetHandle() const
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
	switch (msg)
	{
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
			break;
		case WM_KILLFOCUS:
			m_editor->RaiseSystemEvent();
			break;
		case WM_ACTIVATE:
			m_editor->RaiseSystemEvent();
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYUP:
		case WM_CHAR:
		{
			m_editor->RaiseKeyboardEvent();
			break;
		}
		case WM_MOUSEMOVE:
		{
			const POINTS pt = MAKEPOINTS(lParam);
			m_editor->RaiseMouseEvent();
			break;
		}
		case WM_LBUTTONDOWN:
		{
			m_editor->RaiseMouseEvent();
			break;
		}
		case WM_RBUTTONDOWN:
		{
			m_editor->RaiseMouseEvent();
			break;
		}
		case WM_LBUTTONUP:
		{
			m_editor->RaiseMouseEvent();
			break;
		}
		case WM_RBUTTONUP:
		{
			m_editor->RaiseMouseEvent();
			break;
		}
		case WM_MOUSEWHEEL:
		{
			m_editor->RaiseMouseEvent();
			break;
		}

	}

	return DefWindowProc(handle, msg, wParam, lParam);
}

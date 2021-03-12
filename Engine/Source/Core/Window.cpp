#include "Window.h"
#include <cassert>
Engine::Core::Window* Engine::Core::Window::m_pInstance = nullptr;

Engine::Core::Window::Window(HINSTANCE& instance, std::shared_ptr<EventSystem> event_system, bool fullscreen)
	:m_instance(instance), m_eventSystem(event_system)
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

	//Grab Desktop Resolution
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	m_handle = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		m_className,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,
		desktop.right, desktop.bottom,NULL,NULL,m_instance,NULL);


	if (!m_handle)
	{
		EDITOR_ERROR("Failed To Create Window");
	}
	else
	{
		EDITOR_INFO("Window Created Successfully");
		
		unsigned int width = 0;
		unsigned int height = 0;

		RECT rect;
		if (GetWindowRect(m_handle, &rect))
		{
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;
		}

		assert(width != 0 && height != 0);
		m_initialWidth = width;
		m_initialHeight = height;

		ShowWindow(m_handle, SW_SHOWDEFAULT);
		
		
		ImGui_ImplWin32_Init(m_handle);
	}
	
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT Engine::Core::Window::WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	return m_pInstance->MyWinProc(handle, msg, wParam, lParam);
}

 HWND Engine::Core::Window::GetHandle() 
{
	return m_handle;
}



bool Engine::Core::Window::Update()
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

Engine::Core::Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
	UnregisterClass(m_className,m_instance);
	EDITOR_INFO("Unregistering Window");
	DestroyWindow(m_handle);
	EDITOR_INFO("Destroying Window");
}

const int Engine::Core::Window::GetInitialWidth() const
{
	return m_initialWidth;
}

const int Engine::Core::Window::GetInitialHeight() const
{
	return m_initialHeight;
}

LRESULT Engine::Core::Window::MyWinProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
	if (ImGui_ImplWin32_WndProcHandler(handle, msg, wParam, lParam))
	{
		return true;
	}
	const ImGuiIO* io = nullptr;
	
	if (ImGui::GetCurrentContext())
	{
		io = &ImGui::GetIO();
	}
	
	Engine::Core::EventData data = {};
	data.msg = msg;
	data.lparam = lParam;
	data.wparam = wParam;
	data.handle = handle;

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
		if (io != nullptr && io->WantCaptureMouse)
		{
			break;
		}
		[[fallthrough]];
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYUP:
	case WM_CHAR:
		if (io != nullptr &&  io->WantCaptureKeyboard)
		{
			break;
		}
		[[fallthrough]];
	case WM_SIZE:
	case WM_MOUSELEAVE:
	case WM_CLOSE:
		Event* e = new Event(data);
		m_eventSystem->Push(e);
		break;
	}
		
	return DefWindowProc(handle, msg, wParam, lParam);
}
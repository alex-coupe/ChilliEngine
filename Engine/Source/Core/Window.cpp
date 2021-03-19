#include "Window.h"
#include <cassert>
Engine::Core::Window* Engine::Core::Window::m_pInstance = nullptr;

Engine::Core::Window::Window(HINSTANCE& instance, const std::shared_ptr<Event>& event_dispatcher, std::shared_ptr<Gui::GuiManager>& gui_man, int width, int height)
	:m_instance(instance), m_event(event_dispatcher), m_gui(gui_man)
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
		MessageBox(m_handle, L"Failed To Register Window", L"Chilli Error", MB_ICONWARNING | MB_OK);

	//Grab Desktop Resolution
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	if (AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
		MessageBox(m_handle, L"Failed To Adjust Window Rect", L"Chilli Error", MB_ICONWARNING | MB_OK);

	m_handle = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		m_className,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,
		wr.right - wr.left, wr.bottom - wr.top,NULL,NULL,m_instance,NULL);


	if (!m_handle)
	{
		MessageBox(m_handle, L"Failed To Create Handle", L"Chilli Error", MB_ICONWARNING | MB_OK);
		return;
	}
	
		assert(width != 0 && height != 0);
		m_initialWidth = width;
		m_initialHeight = height;

		ShowWindow(m_handle, SW_SHOWDEFAULT);
		
		
		m_gui->InitWindowsHook(m_handle);
}



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
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
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
	UnregisterClass(m_className,m_instance);
	DestroyWindow(m_handle);
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
	
	m_gui->WndProcHandler(handle,msg,wParam,lParam);
	const auto io = m_gui->GetIO();	

	//Raise Event For The Messages We're Interested In
	switch (msg)
	{
	case WM_CLOSE:
		if (MessageBox(m_handle, L"Really quit?", L"Chilli Engine", MB_OKCANCEL) == IDOK)
		{
			PostQuitMessage(0);
		}
		return 0;
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
		EventData* const e = new EventData{};
		e->msg = msg;
		e->lparam = lParam;
		e->wparam = wParam;
		e->handle = handle;
		m_event->Push(e);
		break;
	}
		
	return DefWindowProc(handle, msg, wParam, lParam);
}

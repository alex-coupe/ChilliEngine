#include "WindowsPlatform.h"
#include <cassert>

using namespace Engine::Core::Platform;

WindowsPlatform* WindowsPlatform::m_pInstance = nullptr;

WindowsPlatform::WindowsPlatform(std::shared_ptr<Gui::GuiManager>& gui_man, int& width, int& height)
	:m_gui(gui_man)
{
	m_pInstance = this;
	WNDCLASSEX wndclass = {};

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = 0;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = GetModuleHandle(nullptr);
	wndclass.lpszClassName = m_className;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hCursor = LoadCursor(GetModuleHandle(nullptr), IDC_ARROW);
	wndclass.hIcon = LoadIcon(GetModuleHandle(nullptr), IDI_APPLICATION);
	wndclass.hIconSm = LoadIcon(GetModuleHandle(nullptr), IDI_APPLICATION);
	wndclass.lpszMenuName = nullptr;

	if (!RegisterClassEx(&wndclass))
		MessageBox(m_handle, L"Failed To Register Window", L"Chilli Error", MB_ICONWARNING | MB_OK);

	m_handle = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		m_className,
		title,
		WS_OVERLAPPEDWINDOW | WS_MAXIMIZE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, GetModuleHandle(nullptr), NULL);

	if (!m_handle)
	{
		MessageBox(m_handle, L"Failed To Create Handle", L"Chilli Error", MB_ICONWARNING | MB_OK);
		return;
	}

	RECT rect = {};

	if (!GetWindowRect(m_handle, &rect))
		MessageBox(m_handle, L"Failed To Get Window Rect", L"Chilli Error", MB_ICONWARNING | MB_OK);

	width = rect.right - rect.left;
	height = rect.bottom - rect.top;

	assert(width != 0 && height != 0);

	if (!MoveWindow(m_handle, 0, 0, width, height, false))
		MessageBox(m_handle, L"Failed To Resize Window", L"Chilli Error", MB_ICONWARNING | MB_OK);

	ShowWindow(m_handle, SW_SHOWMAXIMIZED);

	m_gui->InitWindowsHook(m_handle);
}

LRESULT WindowsPlatform::WndProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return m_pInstance->MyWinProc(handle, msg, wParam, lParam);
}

HWND WindowsPlatform::GetHandle()
{
	return m_handle;
}

WindowsPlatform::~WindowsPlatform()
{
	UnregisterClass(m_className, GetModuleHandle(nullptr));
	DestroyWindow(m_handle);
}

bool WindowsPlatform::Update()
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

LRESULT WindowsPlatform::MyWinProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{

	m_gui->WndProcHandler(handle, msg, wParam, lParam);
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
		if (io != nullptr && io->WantCaptureKeyboard)
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
		DependencyResolver::ResolveDependency<Events>()->Push(e);
		break;
	}

	return DefWindowProc(handle, msg, wParam, lParam);
}

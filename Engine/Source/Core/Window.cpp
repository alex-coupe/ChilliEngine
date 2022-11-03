#include "Window.h"
#include <cassert>

namespace Chilli {
	Window::Window()
	{
		#ifdef _WIN32
			m_platform = std::make_unique<WindowsPlatform>(m_width, m_height);
		#endif 
	}

	void Window::Quit()
	{
		m_platform->Quit();
	}

	void* Window::GetWindowHandle()
	{
#ifdef _WIN32
		return m_platform->GetHandle();
#endif

	}

	bool Window::Update()
	{
#ifdef _WIN32
		return m_platform->Update();
#endif 
	}

	Window::~Window()
	{
		m_platform->~WindowsPlatform();
	}

	const int Window::GetWidth() const
	{
		return m_width;
	}

	const int Window::GetHeight() const
	{
		return m_height;
	}

	void Window::SetTitle(const char* title)
	{
		m_platform->SetTitle(title);
	}
}

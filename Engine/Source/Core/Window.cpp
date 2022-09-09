#include "Window.h"
#include <cassert>

Engine::Core::Window::Window()
{
#ifdef _WIN32
	m_platform = std::make_unique<Platform::WindowsPlatform>(m_width, m_height);
#endif 
}

void* Engine::Core::Window::GetWindowHandle()
{
#ifdef _WIN32
	return m_platform->GetHandle();
#endif

}

bool Engine::Core::Window::Update()
{
#ifdef _WIN32
	return m_platform->Update();
#endif 
}

Engine::Core::Window::~Window()
{
	m_platform->~WindowsPlatform();
}

const int Engine::Core::Window::GetWidth() const
{
	return m_width;
}

const int Engine::Core::Window::GetHeight() const
{
	return m_height;
}

void Engine::Core::Window::SetTitle(const char* title)
{
	m_platform->SetTitle(title);
}

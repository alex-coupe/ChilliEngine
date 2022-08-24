#include "Events.h"

void Engine::Core::Events::ProcessFrame()
{
	if (m_eventBuffer.size() > 0)
	{
		TrimBuffer();
		const auto& e = m_eventBuffer.front();
		m_eventBuffer.pop();
		ReadEvent(e);
		for (const auto& subscriber : m_subscribers)
		{
			if (subscriber.first == static_cast<EventType>(e->msg))
			{
				if (subscriber.second != nullptr)
					subscriber.second();
			}
		}
		delete e;
	}
}

void Engine::Core::Events::TrimBuffer()
{
	while (m_eventBuffer.size() > 16)
	{
		const auto& e = m_eventBuffer.front();
		m_eventBuffer.pop();
		delete e;
	}
}

void Engine::Core::Events::ResetKeyboard()
{
	m_keys.reset();
}

Engine::Core::Events::~Events()
{
	while (!m_eventBuffer.empty())
	{
		const auto& e = m_eventBuffer.front();
		m_eventBuffer.pop();
		delete e;
	}
}

int Engine::Core::Events::GetSystemType()const
{
	return static_cast<int>(SystemTypes::Events);
}

bool Engine::Core::Events::GetInWindow() const
{
	return m_inWindow;
}

void Engine::Core::Events::ClearBuffer()
{
	while (!m_eventBuffer.empty())
	{
		const auto& e = m_eventBuffer.front();
		m_eventBuffer.pop();
		delete e;
	}
}

void Engine::Core::Events::Subscribe(const EventType type, std::function<void()> callback)
{
	m_subscribers.push_back({ type, callback });
}

void Engine::Core::Events::ReadEvent(const EventData* const data_in )
{
	switch (static_cast<EventType>(data_in->msg))
	{
		case EventType::MouseMove:
			m_mouseX = data_in->lparam & 0x0000FFFF;
			m_mouseY = data_in->lparam >> 16;
			if (!m_inWindow)
			{
				SetCapture(data_in->handle);
				m_inWindow = true;
			}
			break;
		case EventType::MouseWheel:
			if (m_inWindow)
				m_wheelDelta = GET_WHEEL_DELTA_WPARAM(data_in->wparam);
			break;
		case EventType::LeftMousePressed:
			if (m_inWindow)
				m_leftMousePressed = true;
			break;
		case EventType::LeftMouseReleased:
			if (m_inWindow)
				m_leftMousePressed = false;
			break;
		case EventType::MouseLeave:
			m_inWindow = false;
			break;
		case EventType::FocusLost:
			ResetKeyboard();
			break;
		case EventType::KeyUp:
		case EventType::SysKeyUp:
			m_keycode = static_cast<unsigned char>(data_in->wparam);
			m_keys[m_keycode] = false;
			break;
		case EventType::KeyDown:
		case EventType::SysKeyDown:
			m_keycode = static_cast<unsigned char>(data_in->wparam);
			m_keys[m_keycode] = true;
			break;
		case EventType::WindowResized:
			RECT rect;
			if (GetWindowRect(data_in->handle, &rect))
			{
				m_screenWidth = static_cast<int64_t>(rect.right) - rect.left;
				m_screenHeight = static_cast<int64_t>(rect.bottom) - rect.top;
			}
			break;
	}
}

const std::pair<const int64_t&, const int64_t&> Engine::Core::Events::GetMousePosition()const
{
	return std::make_pair(std::cref(m_mouseX),std::cref(m_mouseY));
}

const int64_t Engine::Core::Events::GetScreenWidth() const
{
	return m_screenWidth;
}

const int64_t Engine::Core::Events::GetScreenHeight() const
{
	return m_screenHeight;
}

bool Engine::Core::Events::GetMouseLeftPressed() const
{
	return m_leftMousePressed;
}

bool Engine::Core::Events::GetMouseRightPressed() const
{
	return m_rightMousePressed;
}

short Engine::Core::Events::GetMouseWheelDelta() const
{
	return m_wheelDelta;
}

bool Engine::Core::Events::GetKeyPressed(Key key) const
{
	auto code = static_cast<unsigned int>(key);
	return m_keys[code];
}

bool Engine::Core::Events::GetKeyReleased(Key key) const
{
	auto code = static_cast<unsigned int>(key);
	return m_keys[code];
}

void Engine::Core::Events::Push(const EventData* const e)
{
	m_eventBuffer.push(e);
}


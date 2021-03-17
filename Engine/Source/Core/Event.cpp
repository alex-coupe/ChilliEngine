#include "Event.h"

void Engine::Core::Event::ProcessFrame()
{
	if (m_eventBuffer.size() > 0)
	{
		TrimBuffer();
		const auto& e = m_eventBuffer.front();
		m_eventBuffer.pop();
		ReadEvent(e);
		for (const auto& subscriber : m_Subscribers)
		{
			if (std::find(subscriber.first.begin(), subscriber.first.end(), static_cast<EventType>(e->msg)) != subscriber.first.end())
			{
				subscriber.second();
			}
		}

		delete e;
	}
}

void Engine::Core::Event::TrimBuffer()
{
	while (m_eventBuffer.size() > 16)
	{
		const auto& e = m_eventBuffer.front();
		m_eventBuffer.pop();
		delete e;
	}
}

void Engine::Core::Event::ResetKeyboard()
{
	m_keys.reset();
}

Engine::Core::Event::Event(const std::shared_ptr<DependencyResolver<SubSystem>>& m_resolver)
	: SubSystem(m_resolver)
{
}

Engine::Core::Event::~Event()
{
	while (!m_eventBuffer.empty())
	{
		const auto& e = m_eventBuffer.front();
		m_eventBuffer.pop();
		delete e;
	}
}

int Engine::Core::Event::GetHash()const
{
	return static_cast<int>(SystemTypes::EventSystem);
}

bool Engine::Core::Event::GetInWindow() const
{
	return m_inWindow;
}

void Engine::Core::Event::ClearBuffer()
{
	while (!m_eventBuffer.empty())
	{
		const auto& e = m_eventBuffer.front();
		m_eventBuffer.pop();
		delete e;
	}
}

void Engine::Core::Event::Subscribe(const std::vector<EventType>& types, std::function<void()> callback)
{
	m_Subscribers.push_back({ types, callback });
	
}

void Engine::Core::Event::ReadEvent(const EventData* const data_in )
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

const std::pair<const int64_t&, const int64_t&> Engine::Core::Event::GetMousePosition()const
{
	return std::make_pair(std::cref(m_mouseX),std::cref(m_mouseY));
}

const int64_t& Engine::Core::Event::GetScreenWidth() const
{
	return m_screenWidth;
}

const int64_t& Engine::Core::Event::GetScreenHeight() const
{
	return m_screenHeight;
}

bool Engine::Core::Event::GetMouseLeftPressed() const
{
	return m_leftMousePressed;
}

bool Engine::Core::Event::GetMouseRightPressed() const
{
	return m_rightMousePressed;
}

short Engine::Core::Event::GetMouseWheelDelta() const
{
	return m_wheelDelta;
}

bool Engine::Core::Event::GetKeyPressed(Key key) const
{
	auto code = static_cast<unsigned int>(key);
	return m_keys[code];
}

bool Engine::Core::Event::GetKeyReleased(Key key) const
{
	auto code = static_cast<unsigned int>(key);
	return m_keys[code];
}

void Engine::Core::Event::Push(const EventData* const e)
{
	m_eventBuffer.push(e);
}


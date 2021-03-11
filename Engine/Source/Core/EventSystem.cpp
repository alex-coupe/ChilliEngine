#include "EventSystem.h"
#include "Logger.h"


void Engine::Core::EventSystem::ProcessFrame()
{
	if (m_eventBuffer.size() > 0)
	{
		TrimBuffer();
		const auto& e = m_eventBuffer.front();
		m_eventBuffer.pop();
		auto eventType = e->GetType();
		TranslateEvent(eventType, e->m_data);
		for (const auto& subscriber : m_Subscribers)
		{
			if (std::find(subscriber.first.begin(), subscriber.first.end(), eventType) != subscriber.first.end())
			{
				subscriber.second();
			}
		}

		delete e;
	}
}

void Engine::Core::EventSystem::TrimBuffer()
{
	while (m_eventBuffer.size() > 16)
	{
		const auto& e = m_eventBuffer.front();
		m_eventBuffer.pop();
		delete e;
	}
}

Engine::Core::EventSystem::EventSystem(std::shared_ptr<DependencyResolver<EngineSystem>> m_resolver)
	: EngineSystem(m_resolver)
{
}


int Engine::Core::EventSystem::GetHash()
{
	return static_cast<int>(SystemTypes::EventSystem);
}

void Engine::Core::EventSystem::ClearBuffer()
{
	m_eventBuffer = {};
}

void Engine::Core::EventSystem::Subscribe(std::vector<EventType> events, std::function<void()> callback)
{
	m_Subscribers.push_back({ events, callback });
	
}

void Engine::Core::EventSystem::TranslateEvent(EventType type, const EventData& data_in )
{
	switch (type)
	{
		case EventType::MouseMove:
			m_data.mouse_x = data_in.lparam & 0x0000FFFF;
			m_data.mouse_y = data_in.lparam >> 16;
			break;
		case EventType::MouseWheel:
			m_data.wheel_delta = GET_WHEEL_DELTA_WPARAM(data_in.wparam);
			break;
		case EventType::Character:
		case EventType::KeyUp:
		case EventType::SysKeyUp:
		case EventType::KeyDown:
		case EventType::SysKeyDown:
			m_data.keycode = static_cast<unsigned char>(data_in.wparam);
			break;
		case EventType::WindowResized:
			RECT rect;
			if (GetWindowRect(data_in.handle, &rect))
			{
				m_data.screen_width = static_cast<int64_t>(rect.right) - rect.left;
				m_data.screen_height = static_cast<int64_t>(rect.bottom) - rect.top;
			}
			break;
		case EventType::Close:
			Engine::Core::Logger::Kill();
			break;
	}
}

void Engine::Core::EventSystem::Push(Event* e)
{
	m_eventBuffer.push(e);
}

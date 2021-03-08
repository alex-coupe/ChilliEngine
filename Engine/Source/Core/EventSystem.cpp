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
	}
}

void Engine::Core::EventSystem::TrimBuffer()
{
	while (m_eventBuffer.size() > 16)
		m_eventBuffer.pop();
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
	if (type == EventType::MouseMove)
	{
		m_data.mouse_x = data_in.lparam & 0x0000FFFF;
		m_data.mouse_y = data_in.lparam >> 16;
	}
}

void Engine::Core::EventSystem::Push(Event* e)
{
	m_eventBuffer.push(e);
}

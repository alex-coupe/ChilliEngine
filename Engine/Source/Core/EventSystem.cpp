#include "EventSystem.h"
#include "Logger.h"


void Engine::Core::EventSystem::ProcessFrame()
{
	if (m_eventBuffer.size() > 0)
	{
		auto& e = m_eventBuffer.front();
		m_eventBuffer.pop();
		ENGINE_INFO(e->ToString());
	}
}

void Engine::Core::EventSystem::ClearBuffer()
{
	m_eventBuffer = {};
}

size_t Engine::Core::EventSystem::Subscribe(EventCategory category, void* callback)
{
	size_t identifier = 1;
	switch (category)
	{
	case EventCategory::KeyboardEvent:
		identifier += m_keyboardSubscribers.size();
		m_keyboardSubscribers.emplace(identifier,callback);
		return identifier;
	case EventCategory::MouseEvent:
		identifier += m_mouseSubscribers.size();
		m_mouseSubscribers.emplace(identifier, callback);
		return identifier;
	case EventCategory::SystemEvent:
		identifier += m_systemSubscribers.size();
		m_systemSubscribers.emplace(identifier, callback);
		return identifier;
	
	}
	return 0;
}

void Engine::Core::EventSystem::Unsubscribe(EventCategory category, size_t identifier)
{
	switch (category)
	{
	case EventCategory::KeyboardEvent:
		if (auto x = m_keyboardSubscribers.find(identifier); x != m_keyboardSubscribers.end())
		{
			m_keyboardSubscribers.erase(identifier);
		}
		break;
	case EventCategory::MouseEvent:
		if (auto x = m_mouseSubscribers.find(identifier); x != m_mouseSubscribers.end())
		{
			m_keyboardSubscribers.erase(identifier);
		}
		break;
	case EventCategory::SystemEvent:
		if (auto x = m_systemSubscribers.find(identifier); x != m_systemSubscribers.end())
		{
			m_systemSubscribers.erase(identifier);
		}
		break;
	case EventCategory::None:
		break;
	}
}

void Engine::Core::EventSystem::Push(Event* e)
{
	m_eventBuffer.push(e);
}

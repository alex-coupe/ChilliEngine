#include "ChilliEngine.h"

void ChilliEngine::Init()
{
	m_eventSystem = std::make_unique<EventSystem>();
	ENGINE_INFO("Chilli Engine Initialized Successfully");
}

void ChilliEngine::Update()
{
	m_eventSystem->ProcessFrame();
}

void ChilliEngine::RaiseKeyboardEvent(EventData& data)
{
	KeyboardEvent* e = new KeyboardEvent(data);
	m_eventSystem->Push(e);
}

void ChilliEngine::RaiseMouseEvent(EventData& data)
{
	MouseEvent* e = new MouseEvent(data);
	m_eventSystem->Push(e);
}

void ChilliEngine::RaiseSystemEvent(EventData& data)
{
	SystemEvent* e = new SystemEvent(data);
	m_eventSystem->Push(e);
}

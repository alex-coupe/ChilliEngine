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

void ChilliEngine::RaiseKeyboardEvent()
{
	KeyboardEvent* e = new KeyboardEvent;
	m_eventSystem->Push(e);
}

void ChilliEngine::RaiseMouseEvent()
{
	MouseEvent* e = new MouseEvent;
	m_eventSystem->Push(e);
}

void ChilliEngine::RaiseSystemEvent()
{
	SystemEvent* e = new SystemEvent;
	m_eventSystem->Push(e);
}

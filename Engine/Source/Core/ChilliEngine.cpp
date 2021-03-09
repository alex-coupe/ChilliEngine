#include "ChilliEngine.h"

void ChilliEngine::Init()
{
	//Initialise SubSystems
	m_timer = std::make_unique<Timer>();
	m_eventSystem = std::make_unique<EventSystem>();
	ENGINE_INFO("Chilli Engine Initialized Successfully");
	
	//Subscribe To Events
	//m_eventSystem->Subscribe({ EventType::MouseMove }, std::bind(MouseMovedCallback, std::ref(m_eventSystem->m_data.mouse_x), std::ref(m_eventSystem->m_data.mouse_y)));
	
	
}

void ChilliEngine::Update()
{
	m_timer->ProcessFrame();
	m_eventSystem->ProcessFrame();	
	
}

//Bridge Between Window And Engine
void ChilliEngine::RaiseEvent(EventData& data)
{
	Event* e = new Event(data);
	m_eventSystem->Push(e);
}


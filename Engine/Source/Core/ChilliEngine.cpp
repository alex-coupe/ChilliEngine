#include "ChilliEngine.h"

void ChilliEngine::Init(WindowData& window_data)
{
	//Create Systems
	m_resolver = std::make_shared<DependencyResolver<EngineSystem>>();
	m_eventSystem = std::make_shared<EventSystem>(m_resolver);
	m_timer = std::make_shared<Timer>(m_resolver);
	m_renderingSystem = std::make_shared<RenderingSystem>(window_data.handle, window_data.width, window_data.height, m_resolver);
	
	//Register Dependencies	
	m_resolver->Add(m_eventSystem);
	m_resolver->Add(m_timer);
	m_resolver->Add(m_renderingSystem);

	//Initialize SubSystems As Required
	m_renderingSystem->Init();

	ENGINE_INFO("Chilli Engine Initialized Successfully");

}

void ChilliEngine::Update()
{
	m_timer->ProcessFrame();
	m_eventSystem->ProcessFrame();	
	m_renderingSystem->ProcessFrame();
}

//Bridge Between Window And Engine
void ChilliEngine::RaiseEvent(EventData& data)
{
	Event* e = new Event(data);
	m_eventSystem->Push(e);
}


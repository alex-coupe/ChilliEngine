#include "ChilliEngine.h"

bool ChilliEngine::Init(WindowData& window_data)
{
	//Create Systems
	if (m_resolver = std::make_shared<DependencyResolver<EngineSystem>>(); m_resolver == nullptr)
	{
		ENGINE_ERROR("Failed To Initialize Engine System Resolver");
		return false;
	}

	if (m_eventSystem = std::make_shared<EventSystem>(m_resolver); m_eventSystem == nullptr)
	{
		ENGINE_ERROR("Failed To Initialize Event System");
		return false;
	}

	if (m_timerSystem = std::make_shared<Timer>(m_resolver); m_timerSystem == nullptr)
	{
		ENGINE_ERROR("Failed To Initialize Timer System");
		return false;
	}

	m_renderingSystem = std::make_shared<RenderingSystem>(window_data.handle, window_data.width, window_data.height, m_resolver);

	//Register Dependencies	
	m_resolver->Add(m_eventSystem);
	m_resolver->Add(m_timerSystem);
	m_resolver->Add(m_renderingSystem);

	//Initialize SubSystems As Required
	if (!m_renderingSystem->Init())
	{
		ENGINE_ERROR("Failed To Initialize Rendering System");
		return false;
	}

	ENGINE_INFO("Chilli Engine Initialized Successfully");
	return true;
}

void ChilliEngine::Update()
{
	m_timerSystem->ProcessFrame();
	m_eventSystem->ProcessFrame();	
	m_renderingSystem->ProcessFrame();
}

//Bridge Between Window And Engine
void ChilliEngine::RaiseEvent(EventData& data)
{
	Event* e = new Event(data);
	m_eventSystem->Push(e);
}


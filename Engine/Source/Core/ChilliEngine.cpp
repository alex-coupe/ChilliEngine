#include "ChilliEngine.h"

ChilliEngine::ChilliEngine(ImGuiContext* context)
{
	ImGui::SetCurrentContext(context);
}

ChilliEngine::~ChilliEngine()
{
	ImGui::DestroyContext();
}

bool ChilliEngine::Init(HINSTANCE& hInstance, HWND handle, unsigned int width, unsigned int height)
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
	//Register Dependencies	
	m_resolver->Add(m_eventSystem);

	if (m_timerSystem = std::make_shared<Timer>(m_resolver); m_timerSystem == nullptr)
	{
		ENGINE_ERROR("Failed To Initialize Timer System");
		return false;
	}
	m_resolver->Add(m_timerSystem);

	m_renderingSystem = std::make_shared<RenderingSystem>(m_resolver,handle, width, height);
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

void ChilliEngine::RaiseEvent(EventData& data)
{
	if (m_eventSystem != nullptr)
	{
		Event* e = new Event(data);
		m_eventSystem->Push(e);
	}
}



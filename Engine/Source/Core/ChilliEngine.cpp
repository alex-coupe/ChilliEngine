#include "ChilliEngine.h"

ChilliEngine::ChilliEngine(ImGuiContext* context)
{
	ImGui::SetCurrentContext(context);

	//Enable Logging
	Engine::Core::Logger::Init();


}

ChilliEngine::~ChilliEngine()
{
	ImGui::DestroyContext();
}

bool ChilliEngine::Init(HINSTANCE& hInstance)
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

	if (m_window = std::make_unique<Window>(hInstance,m_eventSystem, 1920,1080 ); m_window == nullptr)
	{
		ENGINE_ERROR("Failed To Create Window");
		return false;
	}

	m_renderingSystem = std::make_shared<RenderingSystem>(m_resolver, m_window->GetWidth(), m_window->GetHeight(), m_window->GetHandle());

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

bool ChilliEngine::Update()
{
	while (m_window->Update())
	{
		m_timerSystem->ProcessFrame();
		m_eventSystem->ProcessFrame();
		m_renderingSystem->ProcessFrame();
		return true;
	}
	return false;
}



#include "ChilliEngine.h"
#include <iostream>
#include <Windows.h>

void ChilliEngine::Update()
{
	while (m_window->Update())
	{
		m_timerSystem->ProcessFrame();
		m_eventSystem->ProcessFrame();
		m_renderingSystem->ProcessFrame();
		
	}
}


ChilliEngine::ChilliEngine(HINSTANCE& hInstance)
{
	AllocConsole();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	
	//Create Systems
	if (m_resolver = std::make_shared<DependencyResolver<EngineSystem>>(); m_resolver == nullptr)
		ENGINE_ERROR("Failed To Initialize Engine System Resolver");
		

	if (m_eventSystem = std::make_shared<EventSystem>(m_resolver); m_eventSystem == nullptr)
		ENGINE_ERROR("Failed To Initialize Event System");
		

	if (m_timerSystem = std::make_shared<Timer>(m_resolver); m_timerSystem == nullptr)
		ENGINE_ERROR("Failed To Initialize Timer System");
		
	if (m_window = std::make_unique<Window>(hInstance, m_eventSystem, false); m_window == nullptr)
		ENGINE_ERROR("Failed To Create Window");
		

	m_renderingSystem = std::make_shared<RenderingSystem>(m_resolver, m_window->GetInitialWidth(), m_window->GetInitialHeight(), m_window->GetHandle());


	//Register Dependencies	
	m_resolver->Add(m_eventSystem);
	m_resolver->Add(m_timerSystem);
	m_resolver->Add(m_renderingSystem);

	//Initialize SubSystems As Required

	if (!m_renderingSystem->Init())
		ENGINE_ERROR("Failed To Initialize Rendering System");
}

ChilliEngine::~ChilliEngine()
{
	FreeConsole();
	m_eventSystem.reset();
	m_renderingSystem.reset();
	m_resolver->Flush();
	m_resolver.reset();
	m_timerSystem.reset();
	m_window.reset();
	ImGui::DestroyContext();
}





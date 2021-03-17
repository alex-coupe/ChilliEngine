#include "ChilliEngine.h"
#include <Windows.h>

void ChilliEngine::Update()
{
	while (m_window->Update())
	{
		m_timer->ProcessFrame();
		m_events->ProcessFrame();
		m_renderer->ProcessFrame();
	}
}

ChilliEngine::ChilliEngine(HINSTANCE& hInstance)
{
	m_GuiManager = std::make_shared<GuiManager>();
	
	//Create Systems
	if (m_resolver = std::make_shared<DependencyResolver<SubSystem>>(); m_resolver == nullptr)
		MessageBox(m_window->GetHandle(), L"Failed To Initialize Dependency Resolver", L"Chilli Error", MB_ICONWARNING | MB_OK);
		

	if (m_events = std::make_shared<Event>(m_resolver); m_events == nullptr)
		MessageBox(m_window->GetHandle(), L"Failed To Initialize Event System", L"Chilli Error", MB_ICONWARNING | MB_OK);
		

	if (m_timer = std::make_shared<Timer>(m_resolver); m_timer == nullptr)
		MessageBox(m_window->GetHandle(), L"Failed To Initialize Timing System", L"Chilli Error", MB_ICONWARNING | MB_OK);
		
	if (m_window = std::make_unique<Window>(hInstance, m_events, m_GuiManager); m_window == nullptr)
		MessageBox(m_window->GetHandle(), L"Failed To Create Window", L"Chilli Error", MB_ICONWARNING | MB_OK);
		

	m_renderer = std::make_shared<Renderer>(m_resolver, m_window->GetInitialWidth(), m_window->GetInitialHeight(), m_window->GetHandle(), m_GuiManager);

	m_sceneManager = std::make_shared<SceneManager>(m_resolver);

	m_GuiManager->AddGuiElement(std::bind(&SceneManager::DrawGui, m_sceneManager));

	//Register Dependencies	
	m_resolver->Add(m_events);
	m_resolver->Add(m_timer);
	m_resolver->Add(m_renderer);
	m_resolver->Add(m_sceneManager);

	//Initialize SubSystems As Required

	if (!m_renderer->Init())
		MessageBox(m_window->GetHandle(), L"Failed To Initialize Renderer", L"Chilli Error", MB_ICONWARNING |MB_ABORTRETRYIGNORE);
}

ChilliEngine::~ChilliEngine()
{
	m_renderer.reset();
	m_resolver->Flush();
	m_resolver.reset();
	m_timer.reset();
	m_window.reset();
	m_events.reset();
	m_GuiManager.reset();
}





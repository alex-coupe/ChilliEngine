#include "ChilliEngine.h"
#include <Windows.h>

void ChilliEngine::Update()
{
	while (m_window->Update())
	{
		m_timer->ProcessFrame();
		m_events->ProcessFrame();
		m_renderer->ProcessFrame();
		m_sceneManager->ProcessFrame();
	}
}

ChilliEngine::ChilliEngine(HINSTANCE& hInstance)
{
	m_guiManager = std::make_shared<GuiManager>();
	
	if (m_events = std::make_shared<Event>(); m_events == nullptr)
		MessageBox(m_window->GetHandle(), L"Failed To Initialize Event System", L"Chilli Error", MB_ICONWARNING | MB_OK);
	
	DependencyResolver::Add(m_events);

	if (m_timer = std::make_shared<Timer>(); m_timer == nullptr)
		MessageBox(m_window->GetHandle(), L"Failed To Initialize Timing System", L"Chilli Error", MB_ICONWARNING | MB_OK);
	DependencyResolver::Add(m_timer);

	if (m_window = std::make_unique<Window>(hInstance, m_events, m_guiManager, 900, 700); m_window == nullptr)
		MessageBox(m_window->GetHandle(), L"Failed To Create Window", L"Chilli Error", MB_ICONWARNING | MB_OK);
		
	m_renderer = std::make_shared<Renderer>(m_window->GetInitialWidth(), m_window->GetInitialHeight(), m_window->GetHandle(), m_guiManager);
	DependencyResolver::Add(m_renderer);

	m_sceneManager = std::make_shared<SceneManager>();
	DependencyResolver::Add(m_sceneManager);

	m_guiManager->AddGuiElement(std::bind(&SceneManager::DrawGui, m_sceneManager));

	if (!m_renderer->Init())
		MessageBox(m_window->GetHandle(), L"Failed To Initialize Renderer", L"Chilli Error", MB_ICONWARNING |MB_ABORTRETRYIGNORE);
}

ChilliEngine::~ChilliEngine()
{
	m_sceneManager.reset();
	m_renderer.reset();
	DependencyResolver::Flush();
	m_timer.reset();
	m_window.reset();
	m_events.reset();
	m_guiManager.reset();
}





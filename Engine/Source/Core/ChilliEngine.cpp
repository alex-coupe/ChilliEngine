#include "ChilliEngine.h"

void ChilliEngine::Run()
{
	while (m_window->Update())
	{
		m_timer->ProcessFrame();
		m_events->ProcessFrame();
		m_renderer->ProcessFrame();
		m_sceneManager->ProcessFrame();
	}
}

ChilliEngine::ChilliEngine()
{	
	CHILLI_INFO("Booting Engine");

	m_guiManager = std::make_shared<GuiManager>();
	if (m_guiManager == nullptr)
		CHILLI_ERROR("Failed to create GUI manager");

	if (m_events = std::make_shared<Events>(); m_events == nullptr)
		CHILLI_ERROR("Failed to create events system");
	
	DependencyResolver::Add(m_events);

	if (m_timer = std::make_shared<Timer>(); m_timer == nullptr)
		CHILLI_ERROR("Failed to create timer system");

	DependencyResolver::Add(m_timer);

	if (m_window = std::make_unique<Window>(m_guiManager); m_window == nullptr)
		CHILLI_ERROR("Failed to create window");
		
	m_renderer = std::make_shared<Renderer>(m_window->GetWidth(), m_window->GetHeight(), m_window->GetWindowHandle(), m_guiManager);
	if (m_renderer == nullptr)
		CHILLI_ERROR("Failed to create renderer");

	DependencyResolver::Add(m_renderer);

	m_sceneManager = std::make_shared<SceneManager>();
	if (m_sceneManager == nullptr)
		CHILLI_ERROR("Failed to create scene manager");
	DependencyResolver::Add(m_sceneManager);

	m_guiManager->AddGuiElement(std::bind(&SceneManager::DrawGui, m_sceneManager));

	if (!m_renderer->Init())
		CHILLI_ERROR("Renderer could not init");
}

ChilliEngine::~ChilliEngine()
{
	m_sceneManager.reset();
	m_renderer.reset();
	m_timer.reset();
	m_window.reset();
	m_events.reset();
	m_guiManager.reset();
	DependencyResolver::Flush();
}





#include "ChilliEngine.h"

void ChilliEngine::Run()
{
	while (m_window->Update())
	{
		m_timer->ProcessFrame();
		m_events->ProcessFrame();
		if (m_projectManager->GetCurrentScene()->GetSceneState() == Engine::ResourceSystem::SceneState::Play)
		{ 
			m_physicsManager->ProcessFrame();
		}
		m_renderer->ProcessFrame();
		m_projectManager->ProcessFrame();
	}
}

ChilliEngine::ChilliEngine()
{	
	CHILLI_INFO("Booting Engine");

	GuiManager::Init();

	if (m_events = std::make_shared<Events>(); m_events == nullptr)
		CHILLI_ERROR("Failed to create events system");
	
	DependencyResolver::Add(m_events);

	if (m_timer = std::make_shared<Timer>(); m_timer == nullptr)
		CHILLI_ERROR("Failed to create timer system");

	DependencyResolver::Add(m_timer);

	if (m_window = std::make_unique<Window>(); m_window == nullptr)
		CHILLI_ERROR("Failed to create window");

	if (m_physicsManager = std::make_shared<PhysicsManager>();m_physicsManager == nullptr)
		CHILLI_ERROR("Failed to create physcis manager");
	DependencyResolver::Add(m_physicsManager);
		
	m_renderer = std::make_shared<Renderer>(m_window->GetWidth(), m_window->GetHeight(), m_window->GetWindowHandle());
	if (m_renderer == nullptr)
		CHILLI_ERROR("Failed to create renderer");

	DependencyResolver::Add(m_renderer);

	m_projectManager = std::make_shared<ProjectManager>();
	if (m_projectManager == nullptr)
		CHILLI_ERROR("Failed to create scene manager");
	DependencyResolver::Add(m_projectManager);

	if (!m_renderer->Init())
		CHILLI_ERROR("Renderer could not init");
}

ChilliEngine::~ChilliEngine()
{
	m_projectManager.reset();
	m_renderer.reset();
	m_timer.reset();
	m_window.reset();
	m_events.reset();
	m_physicsManager.reset();
	GuiManager::Shutdown();
	DependencyResolver::Flush();
}





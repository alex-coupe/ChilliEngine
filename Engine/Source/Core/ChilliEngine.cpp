#include "ChilliEngine.h"

void ChilliEngine::Run()
{
	while (m_window->Update())
	{
		m_timer->ProcessFrame();
		m_events->ProcessFrame();
		m_scriptingEngine->ProcessFrame();
		m_projectManager->ProcessFrame();
		m_renderer->ProcessFrame();
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
		
	m_renderer = std::make_shared<Renderer>(m_window->GetWidth(), m_window->GetHeight(), m_window->GetWindowHandle());
	if (m_renderer == nullptr)
		CHILLI_ERROR("Failed to create renderer");

	DependencyResolver::Add(m_renderer);

	m_projectManager = std::make_shared<ProjectManager>();
	if (m_projectManager == nullptr)
		CHILLI_ERROR("Failed to create scene manager");
	DependencyResolver::Add(m_projectManager);

	m_scriptingEngine = std::make_shared<ScriptingEngine>();
	if (m_scriptingEngine == nullptr)
		CHILLI_ERROR("Failed to create scripting engine");
	DependencyResolver::Add(m_scriptingEngine);

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
	m_scriptingEngine.reset();
	GuiManager::Shutdown();
	DependencyResolver::Flush();
}





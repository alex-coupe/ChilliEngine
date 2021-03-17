#pragma once
#include "ChilliDefinitions.h"
#include "Event.h"
#include "Timer.h"
#include "DependencyResolver.h"
#include "SubSystem.h"
#include "Window.h"

#include "../Gui/GuiManager.h"
#include "../SceneManagement/SceneManager.h"
#include "../Rendering/Renderer.h"


using namespace Engine::Core;
using namespace Engine::Rendering;
using namespace Engine::SceneManagement;
using namespace Engine::Gui;

class CHILLI_API ChilliEngine {

public:
	ChilliEngine(HINSTANCE& hInstance);
	~ChilliEngine();
	ChilliEngine(const ChilliEngine&) = delete;
	ChilliEngine& operator=(const ChilliEngine&) = delete;
	ChilliEngine(ChilliEngine&&) = default;
	void Update();
private:
	std::shared_ptr<Event> m_events;
	std::shared_ptr<Timer> m_timer;
	std::shared_ptr<Renderer> m_renderer;
	std::shared_ptr<DependencyResolver<SubSystem>> m_resolver;
	std::unique_ptr<Window> m_window;
	std::shared_ptr<SceneManager> m_sceneManager;
	std::shared_ptr<GuiManager> m_GuiManager;
};
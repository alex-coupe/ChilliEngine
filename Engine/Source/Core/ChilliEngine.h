#pragma once
#include "ChilliDefinitions.h"
#include "Events.h"
#include "Timer.h"
#include "DependencyResolver.h"
#include "SubSystem.h"
#include "Window.h"
#include "../Gui/GuiManager.h"
#include "../ResourceSystem/ProjectManager.h"
#include "../Rendering/Renderer.h"

namespace Chilli {

	class CHILLI_API ChilliEngine {

	public:
		ChilliEngine();
		~ChilliEngine();
		ChilliEngine(const ChilliEngine&) = delete;
		ChilliEngine& operator=(const ChilliEngine&) = delete;
		ChilliEngine(ChilliEngine&&) = default;
		void Run();
	private:
		std::shared_ptr<Events> m_events;
		std::shared_ptr<Timer> m_timer;
		std::shared_ptr<Renderer> m_renderer;
		std::unique_ptr<Window> m_window;
		std::shared_ptr<ProjectManager> m_projectManager;
		std::shared_ptr<GuiManager> m_guiManager;
	};
}
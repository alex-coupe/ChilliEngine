#pragma once
#include "ChilliDefinitions.h"
#include "Logger.h"
#include "EventSystem.h"
#include "Timer.h"
#include "DependencyResolver.h"
#include "EngineSystem.h"
#include "../ImGui/imgui.h"
#include "../Rendering/RenderingSystem.h"
#include "Window.h"

namespace Engine::Core {
	class Logger;
}

using namespace Engine::Core;
using namespace Engine::Rendering;

class CHILLI_API ChilliEngine {

public:
	ChilliEngine(ImGuiContext* context);
	~ChilliEngine();
	bool Init(HINSTANCE& hInstance);
	bool Update();
	
private:
	std::shared_ptr<EventSystem> m_eventSystem;
	std::shared_ptr<Timer> m_timerSystem;
	std::shared_ptr<RenderingSystem> m_renderingSystem;
	std::shared_ptr<DependencyResolver<EngineSystem>> m_resolver;
	std::unique_ptr<Window> m_window;
};
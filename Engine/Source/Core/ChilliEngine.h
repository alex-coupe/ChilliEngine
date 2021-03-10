#pragma once
#include "ChilliDefinitions.h"
#include "Logger.h"
#include "EventSystem.h"
#include "Timer.h"
#include "DependencyResolver.h"
#include "EngineSystem.h"

#include "../Rendering/Renderer.h"

namespace Engine::Core {
	class Logger;
}

using namespace Engine::Core;
using namespace Engine::Renderer;

struct WindowData {
	int64_t width = 0;
	int64_t height = 0;
	HWND handle = 0;
};

class CHILLI_API ChilliEngine {

public:
	void Init(WindowData& window_data);
	void Update();
	void RaiseEvent(EventData& data);
private:
	std::shared_ptr<EventSystem> m_eventSystem;
	std::shared_ptr<Timer> m_timer;
	std::shared_ptr<DependencyResolver<EngineSystem>> m_resolver;
	std::shared_ptr<Renderer> m_renderer;
};
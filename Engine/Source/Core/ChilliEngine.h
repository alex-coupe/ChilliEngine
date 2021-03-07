#pragma once
#include "ChilliDefinitions.h"
#include "Logger.h"
#include "EventSystem.h"

namespace Engine::Core {
	class Logger;
}

using namespace Engine::Core;

CHILLI_CLASS ChilliEngine {

public:
	void Init();
	void Update();
	void RaiseKeyboardEvent(EventData& data);
	void RaiseMouseEvent(EventData& data);
	void RaiseSystemEvent(EventData& data);
private:
	std::unique_ptr<EventSystem> m_eventSystem;
};
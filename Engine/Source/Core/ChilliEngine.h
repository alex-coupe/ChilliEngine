#pragma once
#include "ChilliDefinitions.h"
#include "Logger.h"
#include "EventSystem.h"

namespace Engine::Core {
	class Logger;
}

using namespace Engine::Core;

class CHILLI_API ChilliEngine {

public:
	void Init();
	void Update();
	void RaiseEvent(EventData& data);
private:
	std::unique_ptr<EventSystem> m_eventSystem;
};
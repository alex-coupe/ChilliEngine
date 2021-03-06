#pragma once
#include "ChilliDefinitions.h"
#include "Logger.h"

namespace Engine::Core {
	class Logger;
}

CHILLI_CLASS ChilliEngine {

public:
	void Init();
	void Update();
};
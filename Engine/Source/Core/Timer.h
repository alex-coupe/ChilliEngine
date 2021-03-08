#pragma once
#include "ChilliDefinitions.h"
#include <chrono>

namespace Engine::Core {

	class CHILLI_API Timer
	{
	public:
		Timer();
		void ProcessFrame();
		const float GetDeltaTime();
		std::chrono::high_resolution_clock::duration GetElapsedTime();
	private:
		std::chrono::high_resolution_clock::time_point m_frameStart;
		std::chrono::high_resolution_clock::time_point m_frameEnd;
		std::chrono::high_resolution_clock::time_point m_timeStart;
	};
}


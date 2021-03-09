#pragma once
#include "ChilliDefinitions.h"
#include <chrono>
#pragma warning(disable:4251)
namespace Engine::Core {

	class CHILLI_API Timer
	{
	public:
		Timer();
		void ProcessFrame();
		const float GetDeltaTime();
		const float GetElapsedTime();
	private:
		std::chrono::steady_clock::time_point m_frameStart;
		float m_deltaTime = 0.0f;
		std::chrono::steady_clock::time_point m_frameEnd;
		std::chrono::steady_clock::time_point m_appStart;
	};
}


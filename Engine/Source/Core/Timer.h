#pragma once
#include "ChilliDefinitions.h"
#include <chrono>
#include "SubSystem.h"

#pragma warning(disable:4251)
namespace Chilli {

	class CHILLI_API Timer : public SubSystem
	{
	public:
		Timer();
		void ProcessFrame()override;
		static SystemType GetSystemType();
		const float GetDeltaTime()const;
		const float GetElapsedTime()const;
	private:
		std::chrono::steady_clock::time_point m_frameStart;
		float m_deltaTime = 0.0f;
		std::chrono::steady_clock::time_point m_frameEnd;
		std::chrono::steady_clock::time_point m_appStart;
	};
}


#pragma once
#include "ChilliDefinitions.h"
#include <chrono>
#include "EngineSystem.h"

#pragma warning(disable:4251)
namespace Engine::Core {

	class CHILLI_API Timer : public EngineSystem
	{
	public:
		Timer(const std::shared_ptr<DependencyResolver<EngineSystem>>& resolver);
		void ProcessFrame()override;
		int GetHash()const override;
		const float GetDeltaTime()const;
		const float GetElapsedTime()const;
	private:
		std::chrono::steady_clock::time_point m_frameStart;
		float m_deltaTime = 0.0f;
		std::chrono::steady_clock::time_point m_frameEnd;
		std::chrono::steady_clock::time_point m_appStart;
	};
}


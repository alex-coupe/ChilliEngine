#include "Timer.h"

namespace Chilli {
	Timer::Timer()
		: m_frameStart(std::chrono::steady_clock::now()),
		m_frameEnd(std::chrono::steady_clock::now()),
		m_appStart(std::chrono::steady_clock::now())
	{}

	void Timer::ProcessFrame()
	{
		const auto previousFrame = m_frameStart;
		m_frameStart = std::chrono::steady_clock::now();
		m_frameEnd = std::chrono::steady_clock::now();
		std::chrono::duration<float> deltaTime = m_frameEnd - previousFrame;
		std::chrono::duration<float> deltaMilli = std::chrono::duration_cast<std::chrono::milliseconds>(deltaTime);
		m_deltaTime = deltaMilli.count();
	}

	SystemType Timer::GetSystemType()
	{
		return SystemType::Timer;
	}

	const float Timer::GetDeltaTime()const
	{
		return m_deltaTime;
	}

	const float Timer::GetElapsedTime()const
	{
		const auto currentTime = std::chrono::steady_clock::now();
		const std::chrono::duration<float> elapsedTime = currentTime - m_appStart;
		std::chrono::duration<float> elapsedMilli = std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime);
		return elapsedMilli.count();
	}
}
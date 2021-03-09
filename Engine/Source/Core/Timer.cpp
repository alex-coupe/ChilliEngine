#include "Timer.h"

Engine::Core::Timer::Timer()
	: m_frameStart(std::chrono::steady_clock::now()),
	m_frameEnd(std::chrono::steady_clock::now()),
	m_appStart(std::chrono::system_clock::now())
{
}

void Engine::Core::Timer::ProcessFrame()
{
	const auto previousFrame = m_frameStart;
	m_frameStart = std::chrono::high_resolution_clock::now();
	m_frameEnd = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<float> deltaTime = m_frameEnd - previousFrame;
	m_deltaTime = deltaTime.count();

}

const float Engine::Core::Timer::GetDeltaTime()
{
	return m_deltaTime;
}

const float Engine::Core::Timer::GetElapsedTime()
{
	return m_deltaTime;
}

#include "Timer.h"

Engine::Core::Timer::Timer()
	: m_frameStart(std::chrono::high_resolution_clock::now()),
	m_frameEnd(std::chrono::high_resolution_clock::now()),
	m_timeStart(std::chrono::high_resolution_clock::now())
{
}

void Engine::Core::Timer::ProcessFrame()
{
}

const float Engine::Core::Timer::GetDeltaTime()
{
	return 0.0f;
}

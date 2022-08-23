#include "Application.h"

Application::Application()
{
	m_engine = std::make_unique<ChilliEngine>();
}

Application::~Application()
{
	m_engine.reset();
}

void Application::Run()
{
	return m_engine->Run();
}







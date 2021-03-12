#include "Application.h"
#include <iostream>

Application::Application(HINSTANCE& hInstance)
{
	m_engine = std::make_unique<ChilliEngine>(hInstance);
}

Application::~Application()
{
	m_engine.reset();
}

void Application::Update()
{
	return m_engine->Update();
}







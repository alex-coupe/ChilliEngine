#include "Editor.h"

void Editor::Init()
{
	m_Engine = std::make_unique<ChilliEngine>();
	m_Engine->Init();
	
}

Editor::Editor(HINSTANCE& instance)
{
	m_Window = std::make_unique<Window>(instance);
}

void Editor::Update()
{
	m_Engine->Update();
}

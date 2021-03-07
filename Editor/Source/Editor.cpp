#include "Editor.h"

void Editor::Init()
{
	m_Engine = std::make_unique<ChilliEngine>();
	m_Engine->Init();
	
}


void Editor::Update()
{
	m_Engine->Update();
}

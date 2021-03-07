#include "Editor.h"

void Editor::Init()
{
	m_engine = std::make_unique<ChilliEngine>();
	m_engine->Init();
	
}


void Editor::Update()
{
	m_engine->Update();
}

void Editor::RaiseMouseEvent()
{
	m_engine->RaiseMouseEvent();
}

void Editor::RaiseKeyboardEvent()
{
	m_engine->RaiseKeyboardEvent();
}

void Editor::RaiseSystemEvent()
{
	m_engine->RaiseSystemEvent();
}

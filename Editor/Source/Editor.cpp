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

void Editor::RaiseEvent(EventData& data)
{
	m_engine->RaiseEvent(data);
}



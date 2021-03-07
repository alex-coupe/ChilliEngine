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

void Editor::RaiseMouseEvent(EventData& data)
{
	m_engine->RaiseMouseEvent(data);
}

void Editor::RaiseKeyboardEvent(EventData& data)
{
	m_engine->RaiseKeyboardEvent(data);
}

void Editor::RaiseSystemEvent(EventData& data)
{
	m_engine->RaiseSystemEvent(data);
}

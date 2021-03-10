#include "Editor.h"

void Editor::Init(WindowData& window_data)
{
	m_engine = std::make_unique<ChilliEngine>();
	m_engine->Init(window_data);
	
}

void Editor::Update()
{
	m_engine->Update();
}

void Editor::RaiseEvent(EventData& data)
{
	m_engine->RaiseEvent(data);
}



#include "Editor.h"

bool Editor::Init(WindowData& window_data)
{
	m_engine = std::make_unique<ChilliEngine>();
	return m_engine->Init(window_data);
	
}

void Editor::Update()
{
	m_engine->Update();
}

void Editor::RaiseEvent(EventData& data)
{
	m_engine->RaiseEvent(data);
}



#include "Editor.h"

void Editor::Init(HWND& handle)
{
	m_engine = std::make_unique<ChilliEngine>();
	m_engine->Init(handle);
	
}


void Editor::Update()
{
	m_engine->Update();
}

void Editor::RaiseEvent(EventData& data)
{
	m_engine->RaiseEvent(data);
}



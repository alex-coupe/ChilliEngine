#include "Editor.h"


bool Editor::Init(HINSTANCE& hInstance)
{
	m_engine = std::make_unique<ChilliEngine>();
	if (m_engine->Init(hInstance))
	{
		EDITOR_INFO("Editor Initialized Successfully");
		return true;
	}
	EDITOR_INFO("Editor Could Not Initialize");
	return false;
}

bool Editor::Update()
{
	return m_engine->Update();
}




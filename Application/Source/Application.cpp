#include "Application.h"


bool Application::Init(HINSTANCE& hInstance)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiContext* context = ImGui::GetCurrentContext();

	m_engine = std::make_unique<ChilliEngine>(context);
	if (m_engine->Init(hInstance))
	{
		EDITOR_INFO("Editor Initialized Successfully");
		return true;
	}
	EDITOR_INFO("Editor Could Not Initialize");
	return false;
}

bool Application::Update()
{
	return m_engine->Update();
}




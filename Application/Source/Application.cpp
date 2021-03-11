#include "Application.h"


bool Application::Init(HINSTANCE& hInstance)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiContext* context = ImGui::GetCurrentContext();

	m_engine = std::make_shared<ChilliEngine>(context);
	m_window = std::make_unique<Window>(hInstance, 1920, 1080, m_engine);

	if (m_engine->Init(hInstance, m_window->GetHandle(), m_window->GetWidth(), m_window->GetHeight()))
	{
		EDITOR_INFO("Editor Initialized Successfully");
		return true;
	}
	EDITOR_INFO("Editor Could Not Initialize");
	return false;
}

void Application::Update()
{
	while (m_window->Update())
	{
		m_engine->Update();
	}
}




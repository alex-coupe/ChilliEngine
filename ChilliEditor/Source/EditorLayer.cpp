#include "EditorLayer.h"

Chilli::EditorLayer::EditorLayer()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	m_editorPanels.emplace_back(std::make_shared<MenuBar>());
	//m_editorCamera = std::make_unique<Camera>(1.0f, m_aspectRatio, 0.5f, 100.0f, CameraType::Editor, ProjectionType::Perspective);
	DependencyResolver::ResolveDependency<Renderer>()->SetRenderCamera(m_editorCamera.get());
}

void Chilli::EditorLayer::OnUpdate()
{
	const auto sceneState = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene()->GetSceneState();
	
	switch (sceneState)
	{
	case SceneState::Edit:
		DependencyResolver::ResolveDependency<Renderer>()->SetRenderCamera(m_editorCamera.get());
		break;
	}

}

void Chilli::EditorLayer::OnRender()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	for (const auto& panel : m_editorPanels)
	{
		panel->Draw();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

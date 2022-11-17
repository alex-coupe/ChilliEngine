#include "ToolBar.h"

namespace Chilli {

	ToolBar::ToolBar()
	{
		m_playIcon = std::make_shared<Texture>("Resources/Icons/PlayButton.png");
		m_stopIcon = std::make_shared<Texture>("Resources/Icons/StopButton.png");
		m_simulateIcon = std::make_shared<Texture>("Resources/Icons/SimulateButton.png");
	}

	void ToolBar::DrawGui() const
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		auto projMan = DependencyResolver::ResolveDependency<ProjectManager>();
		auto sceneState = projMan->GetCurrentScene()->GetSceneState();
		float size = ImGui::GetWindowHeight() / 2.0f;
		ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.5f)-size);
		ImGui::SetCursorPosY(ImGui::GetWindowHeight() * 0.45f);
		{
			auto& icon = (sceneState == SceneState::Edit || sceneState == SceneState::Simulate) ? m_playIcon : m_stopIcon;
			if (ImGui::ImageButton(icon->GetShaderResourceView().Get(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
			{
				if (sceneState == SceneState::Edit || sceneState == SceneState::Simulate)
					projMan->PlayCurrentScene();
				else if (sceneState == SceneState::Play)
					projMan->StopCurrentScene();
			}
		}
		
		ImGui::SameLine();

		{
			auto& icon = (sceneState == SceneState::Edit || sceneState == SceneState::Play) ? m_simulateIcon : m_stopIcon;
			if (ImGui::ImageButton(icon->GetShaderResourceView().Get(), ImVec2(size, size), ImVec2(0, 0), ImVec2(1, 1), 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
			{
				if (sceneState == SceneState::Edit || sceneState == SceneState::Play)
					projMan->PlayCurrentScene();
				else if (sceneState == SceneState::Simulate)
					projMan->StopCurrentScene();
			}
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}
}

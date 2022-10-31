#include "MenuBar.h"
#include "Gui/ImGui/imgui.h"

namespace Chilli {

	void MenuBar::DrawGui()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewProject();

				if (ImGui::MenuItem("Open", "Ctrl+O"))
					OpenProject();

				if (ImGui::MenuItem("Save", "Ctrl+S"))
					SaveProject();

				if (ImGui::MenuItem("Save As", "Ctrl+Shift+S"))
					SaveAs();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Scene"))
			{
				auto sceneState = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene()->GetSceneState();

				if (sceneState == SceneState::Edit)
				{
					if (ImGui::MenuItem("Play"))
					{
						DependencyResolver::ResolveDependency<ProjectManager>()->PlayCurrentScene();
					}
				}

				if (sceneState == SceneState::Play)
				{
					if (ImGui::MenuItem("Stop"))
					{
						DependencyResolver::ResolveDependency<ProjectManager>()->StopCurrentScene();
					}
				}

				if (sceneState == SceneState::Edit)
				{
					if (ImGui::MenuItem("Simulate"))
					{
						DependencyResolver::ResolveDependency<ProjectManager>()->PlayCurrentScene();
					}
				}

				if (sceneState == SceneState::Play)
				{
					if (ImGui::MenuItem("Pause"))
					{
						DependencyResolver::ResolveDependency<ProjectManager>()->StopCurrentScene();
					}
				}
				ImGui::EndMenu();
			}
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
				1000.0 / float(ImGui::GetIO().Framerate), float(ImGui::GetIO().Framerate));
			ImGui::EndMainMenuBar();
		}

		//Shortcuts
		bool shift = ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_LeftShift) || ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_RightShift);
		bool control = ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_LeftCtrl) || ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_RightCtrl);

		bool N = ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_N);

		if (control && ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_N))
			NewProject();

		if (control && ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_S))
			SaveProject();

		if (control && ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_O))
			OpenProject();

		if (control && shift && ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_S))
			SaveAs();
	}

	void MenuBar::NewProject()
	{
		DependencyResolver::ResolveDependency<ProjectManager>()->NewProject();
	}

	void MenuBar::SaveProject()
	{	
		if (m_savePath == "")
		{
			nfdchar_t* outPath = NULL;
			nfdresult_t result = NFD_SaveDialog("chilli", NULL, &outPath);
			m_savePath = outPath;
			if (result == NFD_OKAY)
			{
				DependencyResolver::ResolveDependency<ProjectManager>()->SaveProject(outPath);
				free(outPath);
			}
		}
		else
			DependencyResolver::ResolveDependency<ProjectManager>()->SaveProject(m_savePath);

	}

	void MenuBar::OpenProject()
	{
		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_OpenDialog("chilli", NULL, &outPath);
		if (result == NFD_OKAY)
		{
			DependencyResolver::ResolveDependency<ProjectManager>()->LoadProject(outPath);
			free(outPath);
		}
	}

	void MenuBar::SaveAs()
	{
		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_SaveDialog("chilli", NULL, &outPath);
		m_savePath = outPath;
		if (result == NFD_OKAY)
		{
			DependencyResolver::ResolveDependency<ProjectManager>()->SaveProject(outPath);
			free(outPath);
		}
	}
}

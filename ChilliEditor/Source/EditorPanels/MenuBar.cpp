#include "MenuBar.h"

namespace Chilli {
	MenuBar::MenuBar(std::unique_ptr<Window>& window)
		:m_window(window)
	{
	}
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

				if (ImGui::MenuItem("Exit", "Alt-F4"))
					m_window->Quit();

				ImGui::EndMenu();
			}
		}

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
			1000.0 / float(ImGui::GetIO().Framerate), float(ImGui::GetIO().Framerate));
		ImGui::EndMainMenuBar();

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
		DependencyResolver::ResolveDependency<ProjectManager>()->SetProjectNotOpen();
	}

	void MenuBar::SaveProject()
	{	
		auto& fullpath = DependencyResolver::ResolveDependency<ProjectManager>()->GetProjectFilePath();
		if (fullpath == "")
		{
			nfdchar_t* outPath = NULL;
			nfdresult_t result = NFD_SaveDialog("chilli", NULL, &outPath);
			if (result == NFD_OKAY)
			{
				DependencyResolver::ResolveDependency<ProjectManager>()->SaveProject(outPath);
				free(outPath);
			}
		}
		else
			DependencyResolver::ResolveDependency<ProjectManager>()->SaveProject(fullpath);

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
		if (result == NFD_OKAY)
		{
			DependencyResolver::ResolveDependency<ProjectManager>()->SaveProject(outPath);
			free(outPath);
		}
	}
}

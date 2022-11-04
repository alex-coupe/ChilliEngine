#include "SplashPanel.h"

void Chilli::SplashPanel::DrawGui() const
{
	auto projMan = DependencyResolver::ResolveDependency<ProjectManager>();
	bool projOpen = projMan->GetProjectOpen();

	if (!projOpen)
		ImGui::OpenPopup("Splashscreen");

	if (ImGui::BeginPopupModal("Splashscreen", NULL, ImGuiWindowFlags_NoDecoration))
	{
		static char projectNameBuffer[50];
		ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.2f, 1.0f), "New Project");
		ImGui::InputText("Project Name", projectNameBuffer,IM_ARRAYSIZE(projectNameBuffer));
		ImGui::Spacing();
		{
			static nfdchar_t* outPath = NULL;
			static nfdresult_t result = NFD_OKAY;
			if (ImGui::Button("Project Directory"))
			{
				result = NFD_PickFolder(NULL, &outPath);
			}

			ImGui::SameLine();
			ImGui::Text(outPath != NULL ? outPath : "Not Selected");
			ImGui::Spacing();
			if (ImGui::Button("Create Project") && projectNameBuffer[0] != NULL && outPath != NULL)
			{
				projMan->NewProject(projectNameBuffer, outPath);
				free(outPath);
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::Separator();
		ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.2f, 1.0f), "Open Project");
		{
			static nfdchar_t* outPath = NULL;
			static nfdresult_t result = NFD_OKAY;
			if (ImGui::Button("Open"))
			{
				result = NFD_OpenDialog("chilli", NULL, &outPath);
				
				if (outPath != NULL)
				{
					DependencyResolver::ResolveDependency<ProjectManager>()->LoadProject(outPath);
					free(outPath);
				}
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::EndPopup();
	}
}

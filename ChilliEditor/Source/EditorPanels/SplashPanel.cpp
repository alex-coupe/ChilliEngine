#include "SplashPanel.h"

void Chilli::SplashPanel::DrawGui() const
{
	auto projMan = DependencyResolver::ResolveDependency<ProjectManager>();
	bool projOpen = projMan->GetProjectOpen();

	if (!projOpen)
		ImGui::OpenPopup("Splashscreen");

	if (ImGui::BeginPopupModal("Splashscreen", NULL, ImGuiWindowFlags_NoDecoration))
	{
		ImGui::PushFont(GuiManager::GetIO()->Fonts->Fonts[1]);
		float center = ImGui::GetWindowWidth() / 2;
		ImGui::SetCursorPosX(center-70.0f);
		ImGui::Text("Chilli Engine");
		ImGui::PopFont();
		ImGui::Spacing();
		ImGui::Spacing();
		static char projectNameBuffer[50];
		ImGui::SetCursorPosX(center - 130.0f);
		ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.2f, 1.0f), "Name");
		ImGui::SetCursorPosX(center - 130.0f);
		ImGui::PushItemWidth(220.0f);
		ImGui::InputText("##name", projectNameBuffer,IM_ARRAYSIZE(projectNameBuffer));
		ImGui::PopItemWidth();
		ImGui::Spacing();
		ImGui::Spacing();
		{
			static nfdchar_t* outPath = NULL;
			static char dirBuffer[256];
			static nfdresult_t result = NFD_OKAY;

			ImGui::SetCursorPosX(center - 130.0f);
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.2f, 1.0f), "Location");
			ImGui::SetCursorPosX(center - 130.0f);
			ImGui::PushItemWidth(220.0f);
			ImGui::InputText("##location", dirBuffer, IM_ARRAYSIZE(dirBuffer));
			ImGui::PopItemWidth();
			ImGui::SameLine();
			if (ImGui::Button("...", ImVec2{ 25.0f,25.0f }))
			{
				result = NFD_PickFolder(NULL, &outPath);
				strcpy_s(dirBuffer, outPath);
			}			
			ImGui::Spacing();
			ImGui::SetCursorPosX(center - 70.0f);
			if (ImGui::Button("Create") && projectNameBuffer[0] != NULL && outPath != NULL)
			{
				projMan->NewProject(projectNameBuffer, outPath);
				free(outPath);
				ImGui::CloseCurrentPopup();
			}
		}
		
		ImGui::SameLine();
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

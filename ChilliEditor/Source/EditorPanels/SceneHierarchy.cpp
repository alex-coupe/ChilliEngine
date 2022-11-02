#include "SceneHierarchy.h"
#include "../ChilliEditor.h"

void Chilli::SceneHierarchy::DrawGui()
{
	ImGui::Begin("Scene Hierarchy", 0);
	
	if (ImGui::BeginPopupContextItem("add entity popup"))
	{
		static char entityName[50] = "";
		ImGui::Text("Add Entity");
		ImGui::InputText("Entity Name", entityName, IM_ARRAYSIZE(entityName));
		if (ImGui::Button("Ok"))
		{
			ImGui::CloseCurrentPopup();
			ChilliEditor::s_selectedScene->AddEntity(entityName);
			entityName[0] = NULL;
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
			ImGui::CloseCurrentPopup();
		ImGui::EndPopup();
	}
	if (ChilliEditor::s_selectedScene)
	{
		if (ImGui::Button("Add Entity"))
			ImGui::OpenPopup("add entity popup"); 
		ImGui::Separator();
		if (ImGui::TreeNode(ChilliEditor::s_selectedScene->GetName().c_str()))
		{
			for (const auto& entity : ChilliEditor::s_selectedScene->GetEntities())
			{
				if (ImGui::Selectable(entity->GetName().c_str(), ChilliEditor::s_selectedEntity && ChilliEditor::s_selectedEntity->Uuid.Get() == entity->Uuid.Get()))
				{
					ChilliEditor::s_selectedEntity = entity;
				}
			}
			ImGui::TreePop();
		}

		ImGui::OpenPopupOnItemClick("add entity popup", ImGuiPopupFlags_MouseButtonRight);
	}
	ImGui::End();
}

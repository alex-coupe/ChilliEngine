#include "SceneHierarchy.h"
#include "../ChilliEditor.h"

void Chilli::SceneHierarchy::DrawGui()
{
	ImGui::Begin("Scene Hierarchy", 0);
	
	if (ImGui::BeginPopupContextItem("add_entity_popup"))
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
			ImGui::OpenPopup("add_entity_popup"); 
		ImGui::Separator();
		static int option = 0;
		if (ImGui::TreeNode(ChilliEditor::s_selectedScene->GetName().c_str()))
		{
			for (const auto& entity : ChilliEditor::s_selectedScene->GetEntities())
			{
				if (!entity)
					continue;
				ImGui::PushID(static_cast<int>(entity->Uuid.Get()));
				if (ImGui::Selectable(entity->GetName().c_str(), ChilliEditor::s_selectedEntity && ChilliEditor::s_selectedEntity->Uuid.Get() == entity->Uuid.Get()))
				{
					ChilliEditor::s_selectedEntity = entity;
				}
				ImGui::PopID();
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::Selectable("Remove Entity"))
						option = 1;
					ImGui::Separator();
					if (ImGui::Selectable("Duplicate Entity")) 
						option = 2;

					if (option == 1)
					{
						ImGui::CloseCurrentPopup();
						ChilliEditor::s_selectedScene->RemoveEntity(entity->Uuid);
						ChilliEditor::s_selectedEntity = nullptr;
						option = 0;
					}
					if (option == 2)
					{
						ImGui::CloseCurrentPopup();
						ChilliEditor::s_selectedScene->DuplicateEntity(entity);
						option = 0;
					}
					
					ImGui::EndPopup();
				}
			}
			ImGui::TreePop();
		}

		
	}
	ImGui::End();
}

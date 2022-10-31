#include "SceneHierarchy.h"
#include "../ChilliEditor.h"

void Chilli::SceneHierarchy::DrawGui()
{
	ImGui::Begin("Scene Hierarchy", 0);
	static char buffer[50] = "";
	ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer));
	if (ImGui::Button("Add Entity"))
	{
		ChilliEditor::s_selectedScene->AddEntity(buffer);
		buffer[0] = NULL;
	}
	ImGui::Separator();
	if (ChilliEditor::s_selectedScene)
	{
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
	}
	ImGui::End();
}

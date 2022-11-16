#include "SceneHierarchy.h"
#include "../ChilliEditor.h"

namespace Chilli {

	void SceneHierarchy::DrawGui()
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
		static char sceneName[50];
		strcpy_s(sceneName, ChilliEditor::s_selectedScene->GetName().c_str());
		if (ImGui::BeginPopupContextItem("scene_name_popup"))
		{
			ImGui::Text("Edit Scene Name");
			ImGui::InputText("Scene Name", sceneName, IM_ARRAYSIZE(sceneName));
			if (ImGui::Button("Ok"))
			{
				ImGui::CloseCurrentPopup();
				ChilliEditor::s_selectedScene->SetName(sceneName);
				sceneName[0] = NULL;
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
			ImGui::SameLine();
			if (ImGui::Button("Rename Scene"))
				ImGui::OpenPopup("scene_name_popup");

			if (ChilliEditor::s_selectedEntity)
			{
				if (ImGui::Button("Remove Entity"))
				{
					ChilliEditor::s_selectedScene->RemoveEntity(ChilliEditor::s_selectedEntity->Uuid);
					ChilliEditor::s_selectedEntity = nullptr;
				}
				ImGui::SameLine();
				if (ImGui::Button("Duplicate Entity"))
					ChilliEditor::s_selectedScene->DuplicateEntity(ChilliEditor::s_selectedEntity);
			}
			ImGui::Separator();
			
			static int option = 0;
			
			if (ImGui::TreeNodeEx(ChilliEditor::s_selectedScene->GetName().c_str()))
			{
				for (const auto& entity : ChilliEditor::s_selectedScene->GetEntities())
				{
					if (!entity || (entity && entity->HasParent()))
						continue;

					DrawEntityNode(entity);
				}
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}

	void SceneHierarchy::DrawEntityNode(const std::shared_ptr<Entity> entity)const
	{
		
		ImGuiTreeNodeFlags flags = ((ChilliEditor::s_selectedEntity && ChilliEditor::s_selectedEntity->Uuid.Get() == entity->Uuid.Get()) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		if (!entity->HasChildren())
		{
			ImGui::PushID(static_cast<int>(entity->Uuid.Get()));
			if (ImGui::Selectable(entity->GetName().c_str(), ChilliEditor::s_selectedEntity && ChilliEditor::s_selectedEntity->Uuid.Get() == entity->Uuid.Get()))
			{
				ChilliEditor::s_selectedEntity = entity;
			}
			ImGui::PopID();

			if (ImGui::BeginDragDropSource())
			{
				auto id = entity->Uuid.Get();
				ImGui::SetDragDropPayload("ENTITY", &id, sizeof(uint64_t));
				ImGui::EndDragDropSource();
			}
		}
		
		if (entity->HasChildren())
		{
			if (ImGui::TreeNodeEx((void*)entity->Uuid.Get(), flags, entity->GetName().c_str()))
			{
				for (const auto& child : entity->GetChildren())
				{
					DrawEntityNode(child);
				}	
				ImGui::TreePop();
			}
			if (ImGui::IsItemClicked())
			{
				ChilliEditor::s_selectedEntity = entity;
			}
			if (ImGui::BeginDragDropSource())
			{
				auto id = entity->Uuid.Get();
				ImGui::SetDragDropPayload("ENTITY", &id, sizeof(uint64_t));
				ImGui::EndDragDropSource();
			}
		}	
	}
}

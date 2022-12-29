#include "EntityInspector.h"
#include "../ChilliEditor.h"

namespace Chilli {

	void EntityInspector::DrawGui()
	{
		ImGui::Begin("Inspector");
		if (ChilliEditor::s_selectedEntity)
		{
			if (ImGui::BeginPopup("addComponent"))
			{
				ImGui::Text("Add Component");
				ImGui::Separator();

				for (int i = 0; i < IM_ARRAYSIZE(m_componentsList); i++)
				{
					if (ImGui::Selectable(m_componentsList[i]))
					{
						ChilliEditor::s_selectedEntity->AddComponent((ComponentType)i);
					}
				}
				ImGui::EndPopup();
			}
			
			if (ImGui::BeginPopupContextItem("add tag popup"))
			{
				static char tagBuffer[50] = "";
				ImGui::Text("Add Tag");
				ImGui::InputText("Tag Name", tagBuffer, IM_ARRAYSIZE(tagBuffer));
				if (ImGui::Button("Ok"))
				{
					ImGui::CloseCurrentPopup();
					m_tags.emplace_back(std::string(tagBuffer));
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
					ImGui::CloseCurrentPopup();
				ImGui::EndPopup();
			}

			for (const auto& component : ChilliEditor::s_selectedEntity->GetComponents())
			{
				if (component->GetComponentType() == ComponentType::ID)
				{
					const auto& idComp = std::static_pointer_cast<IDComponent>(component);
					static char nameBuffer[50];
					strcpy_s(nameBuffer, idComp->GetName().c_str());
					ImGui::InputText("##name", nameBuffer, IM_ARRAYSIZE(nameBuffer));
					ImGui::SameLine();
					if (ImGui::Button("+",ImVec2(25.0f,25.0f)))
						ImGui::OpenPopup("addComponent");

					if (nameBuffer != idComp->GetName().c_str())
						idComp->SetName(nameBuffer);

					ImGui::Text("Tag");
					ImGui::PushID("Tag");
					bool noneSelected = idComp->GetTag() == "";
					if (ImGui::BeginCombo("", noneSelected ? "None" : idComp->GetTag().c_str()))
					{	
						if (ImGui::Checkbox("None", &noneSelected))
						{
							idComp->SetTag("");
						}
						for (int i = 0; i < m_tags.size(); i++)
						{
							bool selected = m_tags[i] == idComp->GetTag();
							if (ImGui::Checkbox(m_tags[i].c_str(), &selected))
							{
								idComp->SetTag(m_tags[i]);
							}
						}
						ImGui::EndCombo();
					}
					ImGui::PopID();
					if (ImGui::Button("Add Tag"))
						ImGui::OpenPopup("add tag popup");

					ImGui::Text("Parent");
					ImGui::PushID("Parent");
					if (ImGui::BeginCombo("", ChilliEditor::s_selectedEntity->GetParentId() != 0 ? 
						ChilliEditor::s_selectedEntity->GetParent()->GetName().c_str() : "None"
					))
					{
						bool noneSelected = ChilliEditor::s_selectedEntity->GetParentId() == 0;
						if (ImGui::Checkbox("None", &noneSelected))
						{
							ChilliEditor::s_selectedEntity->SetParent(0);
						}
						for (const auto& ent: ChilliEditor::s_selectedScene->GetEntities())
						{
							if (ent->Uuid == ChilliEditor::s_selectedEntity->Uuid)
								continue;

							if (ChilliEditor::s_selectedEntity->HasChild(ent->Uuid))
								continue;

							bool selected = ent->Uuid == ChilliEditor::s_selectedEntity->GetParentId();
							if (ImGui::Checkbox(ent->GetName().c_str(), &selected))
							{
								ChilliEditor::s_selectedEntity->SetParent(ent->Uuid);
							}
						}
						ImGui::EndCombo();
					}
					ImGui::PopID();
					ImGui::Spacing();
					continue;
				}
				
				if (ImGui::TreeNodeEx(component->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth))
				{
					switch (component->GetComponentType())
					{
						case ComponentType::Mesh:
							DrawMeshComponentGui(component);
						break;
						case ComponentType::RigidBody2D:
							DrawRigidBody2DComponentGui(component);
						break;
						case ComponentType::Transform:
							DrawTransformComponentGui(component);
						break;
						case ComponentType::BoxCollider2D:
							DrawBoxColliderComponentGui(component);
						break;
						case ComponentType::CircleCollider:
							DrawCircleColliderComponentGui(component);
						break;
						case ComponentType::Camera:
							DrawCameraComponentGui(component);
						break;
						case ComponentType::Light:
							DrawLightComponentGui(component);
						break;
						case ComponentType::Script:
							DrawScriptComponentGui(component);
						break;
						case ComponentType::Sprite:
							DrawSpriteComponentGui(component);
							break;
					}
					ImGui::TreePop();
				}
			}
		}
		ImGui::End();
	}

	void EntityInspector::DrawVec3Control(const char* label, DirectX::XMFLOAT3& values, float resetValue)
	{
		auto boldFont = GuiManager::GetIO()->Fonts->Fonts[2];
		ImGui::PushID(label);
		ImGui::Text(label);
		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y + 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f,lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));

		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##x", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##y", &values.y, 0.1f,0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##z", &values.z, 0.1f,0.0f,0.0f,"%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::PopStyleVar();
		ImGui::PopID();
	}

	void EntityInspector::DrawVec2Control(const char* label, DirectX::XMFLOAT2& values, float resetValue)
	{
		auto boldFont = GuiManager::GetIO()->Fonts->Fonts[2];
		ImGui::PushID(label);
		ImGui::Text(label);
		ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y + 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f,lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));

		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##x", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();

		ImGui::PopStyleColor(3);
		ImGui::SameLine();
		ImGui::DragFloat("##y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::PopStyleVar();
		ImGui::PopID();
	}

	void EntityInspector::DrawTransformComponentGui(const std::shared_ptr<Component> comp)
	{
		const auto& transformComp = std::static_pointer_cast<TransformComponent>(comp);
		ImGui::Spacing();
		DrawVec3Control("Translation", transformComp->Translation());
		ImGui::Spacing();
		DrawVec3Control("Rotation", transformComp->Rotation());
		ImGui::Spacing();
		DrawVec3Control("Scale", transformComp->Scale(),1.0f);
		ImGui::Spacing();
	}

	void EntityInspector::DrawScriptComponentGui(const std::shared_ptr<Component> comp)
	{
		const auto& scriptComp = std::static_pointer_cast<ScriptComponent>(comp);
		
		ImGui::Spacing();
		ImGui::Text("Script");
		ImGui::PushItemWidth(180.0f);
		if (ImGui::BeginCombo("##mesh", scriptComp->GetScriptName() == "" ? "None" : scriptComp->GetScriptName().c_str()))
		{
			bool noneSelected = scriptComp->GetScriptName() == "";
			if (ImGui::Checkbox("None", &noneSelected))
			{
				scriptComp->SetScript("");
				ScriptInstanceRepository::RemoveScriptInstance(ChilliEditor::s_selectedEntity->Uuid.Get());
			}

			for (const auto& script : ScriptEngine::GetAvailableScripts())
			{
				bool selected = script->GetScriptName() == scriptComp->GetScriptName();
				if (ImGui::Checkbox(script->GetScriptName().c_str(), &selected))
				{
					scriptComp->SetScript(script->GetScriptName());
					ScriptInstanceRepository::RemoveScriptInstance(ChilliEditor::s_selectedEntity->Uuid.Get());
					ScriptInstanceRepository::MakeScriptInstance(script->GetScriptName(), ChilliEditor::s_selectedEntity->Uuid.Get());
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_SCRIPT"))
			{
				const char* name = (const char*)payload->Data;
				scriptComp->SetScript(name);
				ScriptInstanceRepository::RemoveScriptInstance(ChilliEditor::s_selectedEntity->Uuid.Get());
				ScriptInstanceRepository::MakeScriptInstance(name, ChilliEditor::s_selectedEntity->Uuid.Get());
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::PopItemWidth();
		ImGui::Spacing();
		ImGui::Spacing();
		if (scriptComp->GetScriptName() != "")
		{
			const auto& scriptInstance = ScriptInstanceRepository::GetScriptInstanceByEntityId(ChilliEditor::s_selectedEntity->Uuid.Get());
			auto& fields = scriptInstance->GetFields();
			for (const auto& [name, field] : fields)
			{
				switch (field.Type)
				{
				case FieldType::Float:
				{
					float data = scriptInstance->GetFieldValue<float>(name);
					ImGui::PushID(field.Name.c_str());
					ImGui::Text(field.Name.c_str());
					ImGui::PushItemWidth(120.0f);
					if (ImGui::DragFloat("", &data))
					{
						scriptInstance->SetFieldValue(name, data);
					}
					ImGui::PopItemWidth();
					ImGui::PopID();
					ImGui::Spacing();
				}
				break;
				case FieldType::Bool:
				{
					bool data = scriptInstance->GetFieldValue<bool>(name);
					ImGui::PushID(field.Name.c_str());
					ImGui::Text(field.Name.c_str());
					if (ImGui::Checkbox("", &data))
					{
						scriptInstance->SetFieldValue(name, data);
					}
					ImGui::PopID();
				}
				break;
				case FieldType::Double:
				{
					double data = scriptInstance->GetFieldValue<double>(name);
					ImGui::PushID(field.Name.c_str());
					ImGui::Text(field.Name.c_str());
					ImGui::PushItemWidth(120.0f);
					if (ImGui::InputDouble("", &data))
					{
						scriptInstance->SetFieldValue(name, data);
					}
					ImGui::PopItemWidth();
					ImGui::PopID();
				}
				break;
				case FieldType::Entity:
				{
					UUID data = scriptInstance->GetFieldValue<UUID>(name);
					ImGui::PushID(field.Name.c_str());
					ImGui::Text(field.Name.c_str());
					ImGui::PushItemWidth(120.0f);
					auto entity = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene()->GetEntityByUUID(data);
					ImGui::Text(data != 0 && entity ? entity->GetName().c_str() : "None");
					if (entity)
					{
						ImGui::SameLine();
						if (ImGui::Button("Clear"))
							scriptInstance->SetFieldValue(name, 0);
					}
					ImGui::Spacing();
					ImGui::Spacing();
					if (ImGui::BeginDragDropTarget())
					{
						if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY"))
						{
							const uint64_t* id = (const uint64_t*)payload->Data;
							scriptInstance->SetFieldValue(name, *id);
						}
						ImGui::EndDragDropTarget();
					}
					ImGui::PopItemWidth();
					ImGui::PopID();
					
				}
				break;
				case FieldType::Int:
				{
					int data = scriptInstance->GetFieldValue<int>(name);
					ImGui::PushID(field.Name.c_str());
					ImGui::Text(field.Name.c_str());
					ImGui::PushItemWidth(120.0f);
					if (ImGui::InputInt("", &data))
					{
						scriptInstance->SetFieldValue(name, data);
					}
					ImGui::PopItemWidth();
					ImGui::PopID();
				}
				break;
				
				case FieldType::Vector2:
				{
					DirectX::XMFLOAT2 data = scriptInstance->GetFieldValue<DirectX::XMFLOAT2>(name);
					auto boldFont = GuiManager::GetIO()->Fonts->Fonts[2];
					ImGui::PushID(field.Name.c_str());
					ImGui::Text(field.Name.c_str());
					ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
					float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y + 2.0f;
					ImVec2 buttonSize = { lineHeight + 3.0f,lineHeight };

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));

					ImGui::PushFont(boldFont);
					if (ImGui::Button("X", buttonSize))
					{
						data.x = 0.0f;
						scriptInstance->SetFieldValue(name, data);
					}
					ImGui::PopFont();

					ImGui::PopStyleColor(3);
					ImGui::SameLine();
					if (ImGui::DragFloat("##x", &data.x, 0.1f, 0.0f, 0.0f, "%.2f"))
						scriptInstance->SetFieldValue(name, data);
					ImGui::PopItemWidth();
					ImGui::SameLine();

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));

					ImGui::PushFont(boldFont);
					if (ImGui::Button("Y", buttonSize))
					{
						data.y = 0.0f;
						scriptInstance->SetFieldValue(name, data);
					}
					ImGui::PopFont();

					ImGui::PopStyleColor(3);
					ImGui::SameLine();
					if (ImGui::DragFloat("##y", &data.y, 0.1f, 0.0f, 0.0f, "%.2f"))
						scriptInstance->SetFieldValue(name, data);
					ImGui::PopItemWidth();
					ImGui::PopStyleVar();
					ImGui::PopID();
					ImGui::Spacing();
				}
				break;
				case FieldType::Vector3:
				{
					DirectX::XMFLOAT3 data = scriptInstance->GetFieldValue<DirectX::XMFLOAT3>(name);
					auto boldFont = GuiManager::GetIO()->Fonts->Fonts[2];
					ImGui::PushID(field.Name.c_str());
					ImGui::Text(field.Name.c_str());
					ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
					ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
					float lineHeight = ImGui::GetFontSize() + ImGui::GetStyle().FramePadding.y + 2.0f;
					ImVec2 buttonSize = { lineHeight + 3.0f,lineHeight };

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));

					ImGui::PushFont(boldFont);
					if (ImGui::Button("X", buttonSize))
					{
						data.x = 0.0f;
						scriptInstance->SetFieldValue(name, data);
					}
					ImGui::PopFont();

					ImGui::PopStyleColor(3);
					ImGui::SameLine();
					if (ImGui::DragFloat("##x", &data.x, 0.1f, 0.0f, 0.0f, "%.2f"))
						scriptInstance->SetFieldValue(name, data);
					ImGui::PopItemWidth();
					ImGui::SameLine();

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.8f, 0.3f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.8f, 0.1f, 0.15f, 1.0f));

					ImGui::PushFont(boldFont);
					if (ImGui::Button("Y", buttonSize))
					{
						data.y = 0.0f;
						scriptInstance->SetFieldValue(name, data);
					}
					ImGui::PopFont();

					ImGui::PopStyleColor(3);
					ImGui::SameLine();
					if (ImGui::DragFloat("##y", &data.y, 0.1f, 0.0f, 0.0f, "%.2f"))
						scriptInstance->SetFieldValue(name, data);
					ImGui::PopItemWidth();
					ImGui::SameLine();

					ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.35f, 0.9f, 1.0f));
					ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.25f, 0.8f, 1.0f));

					ImGui::PushFont(boldFont);
					if (ImGui::Button("Z", buttonSize))
					{
						data.z = 0.0f;
						scriptInstance->SetFieldValue(name, data);
					}
					ImGui::PopFont();

					ImGui::PopStyleColor(3);
					ImGui::SameLine();
					if (ImGui::DragFloat("##z", &data.z, 0.1f, 0.0f, 0.0f, "%.2f"))
						scriptInstance->SetFieldValue(name, data);
					ImGui::PopItemWidth();
					ImGui::SameLine();
					ImGui::PopStyleVar();
					ImGui::PopID();
					ImGui::Spacing();
				}
				break;
				case FieldType::Vector4:
				{
					DirectX::XMFLOAT4 data = scriptInstance->GetFieldValue<DirectX::XMFLOAT4>(name);
					float* vec4[4] = { &data.x,&data.y, &data.z, &data.w };
					ImGui::PushID(field.Name.c_str());
					ImGui::Text(field.Name.c_str());
					ImGui::PushItemWidth(120.0f);
					if (ImGui::ColorEdit4("", vec4[0]))
					{
						scriptInstance->SetFieldValue(name, data);
					}
					ImGui::PopItemWidth();
					ImGui::PopID();
					ImGui::Spacing();
				}
				break;
				}

			}
		}
		ImGui::Spacing();
		if (ImGui::Button("Remove Component"))
		{
			ChilliEditor::s_selectedEntity->RemoveComponent(ComponentType::Script);
		}
	}

	void EntityInspector::DrawMeshComponentGui(const std::shared_ptr<Component> comp)
	{
		const auto& projManager = DependencyResolver::ResolveDependency<ProjectManager>();
		const auto& meshComp = std::static_pointer_cast<MeshComponent>(comp);
		ImGui::Spacing();
		ImGui::Text("Mesh");
		ImGui::PushItemWidth(180.0f);
		if (ImGui::BeginCombo("##mesh", meshComp->meshUuid == 0 ? "None" : meshComp->GetMesh()->GetName().stem().string().c_str()))
		{
			bool noneSelected = meshComp->meshUuid == 0;
			if (ImGui::Checkbox("None", &noneSelected))
			{
				meshComp->meshUuid = 0;
			}

			for (const auto& mesh : projManager->GetMeshes())
			{
				bool selected = mesh.first == meshComp->meshUuid;
				if (ImGui::Checkbox(mesh.second->GetName().stem().string().c_str(), &selected))
				{
					meshComp->meshUuid = mesh.first;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_MESH"))
			{
				const uint64_t* id = (const uint64_t*)payload->Data;
				meshComp->meshUuid = *id;
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::Spacing();
		ImGui::Text("Material");
		ImGui::PushItemWidth(180.0f);
		if (ImGui::BeginCombo("##material", meshComp->materialUuid == 1 ? "Default" : meshComp->GetMaterial().Name.c_str()))
		{
			bool noneSelected = meshComp->materialUuid == 1;
			if (ImGui::Checkbox("Default", &noneSelected))
			{
				meshComp->materialUuid = 1;
			}
			
			for (const auto& material : projManager->GetMaterials())
			{
				if (material.first == 1)
					continue;

				bool selected = material.first == meshComp->materialUuid;
				if (ImGui::Checkbox(material.second.Name.c_str(), &selected))
				{
					meshComp->materialUuid = material.first;
				}
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_MATERIAL"))
			{
				const uint64_t* id = (const uint64_t*)payload->Data;
				meshComp->materialUuid = *id;
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::Button("Remove Component"))
		{
			ChilliEditor::s_selectedEntity->RemoveComponent(ComponentType::Mesh);
		}
	}

	void EntityInspector::DrawRigidBody2DComponentGui(const std::shared_ptr<Component> comp)
	{
		const auto& rb2d = std::static_pointer_cast<RigidBody2DComponent>(comp);
		const char* bodyTypeOptions[] = { "Static","Kinematic","Dynamic" };
		const char* currentBodyTypeSelected = bodyTypeOptions[(int)rb2d->GetBodyType()];
		ImGui::Spacing();
		ImGui::Text("Body Type");
		ImGui::PushItemWidth(130.0f);
		if (ImGui::BeginCombo("##bodytype", currentBodyTypeSelected))
		{
			for (int i = 0; i <= 2; i++)
			{
				bool isSelected = currentBodyTypeSelected == bodyTypeOptions[i];
				if (ImGui::Selectable(bodyTypeOptions[i], isSelected))
				{
					currentBodyTypeSelected = bodyTypeOptions[i];
					rb2d->SetBodyType((BodyType)i);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::Spacing();
		ImGui::Text("Fixed Rotation");
		ImGui::Checkbox("##fixrot", rb2d->GetFixedRotation());
		ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::Button("Remove Component"))
		{
			ChilliEditor::s_selectedEntity->RemoveComponent(ComponentType::RigidBody2D);
		}
	}

	void EntityInspector::DrawCircleColliderComponentGui(const std::shared_ptr<Component> comp)
	{
		auto circleCollider = std::static_pointer_cast<CircleColliderComponent>(comp);
		ImGui::Spacing();
		ImGui::Text("Radius");
		ImGui::PushItemWidth(120.0f);
		ImGui::DragFloat("##radius", &circleCollider->GetRadius(), 0.5f);
		ImGui::PopItemWidth();
		ImGui::Spacing();
		DrawVec2Control("Offset", circleCollider->GetOffset());
		ImGui::Spacing();
		ImGui::Text("Density");
		ImGui::PushItemWidth(120.0f);
		ImGui::DragFloat("##density", &circleCollider->GetDensity(), 0.01f, 0.0f, 1.0f);
		ImGui::PopItemWidth();
		ImGui::Spacing();
		ImGui::Text("Friction");
		ImGui::PushItemWidth(120.0f);
		ImGui::DragFloat("##friction", &circleCollider->GetFriction(), 0.01f, 0.0f, 1.0f);
		ImGui::PopItemWidth();
		ImGui::Spacing();
		ImGui::Text("Restitution");
		ImGui::PushItemWidth(120.0f);
		ImGui::DragFloat("##restitution", &circleCollider->GetRestitution(), 0.01f, 0.0f, 1.0f);
		ImGui::PopItemWidth();
		ImGui::Spacing();
		ImGui::Text("Restitution Threshold");
		ImGui::PushItemWidth(120.0f);
		ImGui::DragFloat("##restitutionthreshold", &circleCollider->GetRestituitonThreshold(), 0.01f, 0.0f);
		ImGui::PopItemWidth();
		ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::Button("Remove Component"))
		{
			ChilliEditor::s_selectedEntity->RemoveComponent(ComponentType::CircleCollider);
		}
	}

	void EntityInspector::DrawBoxColliderComponentGui(const std::shared_ptr<Component> comp)
	{
		auto bc2d = std::static_pointer_cast<BoxCollider2DComponent>(comp);
		DrawVec2Control("Size", bc2d->GetSize(), 1.0f);
		ImGui::Spacing();
		DrawVec2Control("Offset", bc2d->GetOffset());
		ImGui::Spacing();
		ImGui::Spacing();
		ImGui::Text("Density");
		ImGui::PushItemWidth(120.0f);
		ImGui::DragFloat("##density", &bc2d->GetDensity(), 0.01f, 0.0f, 1.0f);
		ImGui::PopItemWidth();
		ImGui::Text("Friction");
		ImGui::PushItemWidth(120.0f);
		ImGui::DragFloat("##friction", &bc2d->GetFriction(), 0.01f, 0.0f, 1.0f);
		ImGui::PopItemWidth();
		ImGui::Text("Restitution");
		ImGui::PushItemWidth(120.0f);
		ImGui::DragFloat("##restitution", &bc2d->GetRestitution(), 0.01f, 0.0f, 1.0f);
		ImGui::PopItemWidth();
		ImGui::Text("Restitution Threshold");
		ImGui::PushItemWidth(120.0f);
		ImGui::DragFloat("##restitutionthreshold", &bc2d->GetRestituitonThreshold(), 0.01f, 0.0f);
		ImGui::PopItemWidth();
		ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::Button("Remove Component"))
		{
			ChilliEditor::s_selectedEntity->RemoveComponent(ComponentType::RigidBody2D);
		}
	}

	void EntityInspector::DrawLightComponentGui(const std::shared_ptr<Component> comp)
	{
		auto light = std::static_pointer_cast<LightComponent>(comp);
		const char* lightTypeOptions[] = { "Directional Light","Point Light", "Spotlight" };
		const char* currentLightTypeSelected = lightTypeOptions[(int)light->GetLightType()];
		ImGui::Spacing();
		ImGui::Text("Light Type");
		if (ImGui::BeginCombo("##lighttype", currentLightTypeSelected))
		{
			for (int i = 0; i <= 2; i++)
			{
				bool isSelected = currentLightTypeSelected == lightTypeOptions[i];
				if (ImGui::Selectable(lightTypeOptions[i], isSelected))
				{
					currentLightTypeSelected = lightTypeOptions[i];
					light->SetType((LightType)i);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::Spacing();
		ImGui::Text("Ambient");
		ImGui::ColorEdit3("##ambient", &light->Ambient().x);
		ImGui::Text("Diffuse");
		ImGui::ColorEdit3("##diffuse", &light->Diffuse().x);
		ImGui::Text("Specular");
		ImGui::ColorEdit3("##specular", &light->Specular().x);
		if (light->GetLightType() != LightType::DirectionalLight)
		{
			ImGui::Spacing();
			ImGui::Text("Attenuation");
			ImGui::Spacing();
			ImGui::Text("Linear");
			ImGui::PushItemWidth(120.0f);
			ImGui::DragFloat("##linear", &light->Linear(), 0.01f, 0.0f, 1.0f);
			ImGui::PopItemWidth();
			ImGui::Text("Constant");
			ImGui::PushItemWidth(120.0f);
			ImGui::DragFloat("##constant", &light->Constant(), 0.01f, 0.0f, 1.0f);
			ImGui::PopItemWidth();
			ImGui::Text("Quadratic");
			ImGui::PushItemWidth(120.0f);
			ImGui::DragFloat("##quadratic", &light->Quadratic(), 0.01f, 0.0f, 1.0f);
			ImGui::PopItemWidth();
		}
		if (light->GetLightType() == LightType::Spotlight)
		{
			ImGui::Spacing();
			ImGui::Text("Cut Off");
			ImGui::Spacing();
			ImGui::Text("Inner");
			ImGui::PushItemWidth(120.0f);
			ImGui::DragFloat("##inner", &light->InnerCutOff(), 0.5f, 0.0f, 50.0f);
			ImGui::PopItemWidth();
			ImGui::Text("Outer");
			ImGui::PushItemWidth(120.0f);
			ImGui::DragFloat("##outer", &light->OuterCutOff(), 0.5f, 0.0f, 50.0f);
			ImGui::PopItemWidth();
		}
		ImGui::Spacing();
		ImGui::Spacing();

		if (ImGui::Button("Remove Component"))
		{
			ChilliEditor::s_selectedEntity->RemoveComponent(ComponentType::Light);
		}
	}

	void EntityInspector::DrawCameraComponentGui(const std::shared_ptr<Component> comp)
	{
		auto camera = std::static_pointer_cast<CameraComponent>(comp);
		ImGui::Spacing();
		ImGui::Text("Projection Type");
		ImGui::PushItemWidth(180.0f);
		const char* projectionTypeOptions[] = { "Perspective","Orthographic" };
		const char* currentProjectionTypeSelected = projectionTypeOptions[(int)camera->GetProjectionType()];
		if (ImGui::BeginCombo("##projectiontype", currentProjectionTypeSelected))
		{
			for (int i = 0; i <= 1; i++)
			{
				bool isSelected = currentProjectionTypeSelected == projectionTypeOptions[i];
				if (ImGui::Selectable(projectionTypeOptions[i], isSelected))
				{
					currentProjectionTypeSelected = projectionTypeOptions[i];
					camera->SetProjectionType((ProjectionType)i);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopItemWidth();
		ImGui::Spacing();
		ImGui::Text("Fov");
		ImGui::PushItemWidth(120.0f);
		ImGui::DragFloat("##fov", &camera->GetFov(), 0.5f);
		ImGui::PopItemWidth();
		ImGui::Text("Near Clip");
		ImGui::PushItemWidth(120.0f);
		ImGui::DragFloat("##nearclip", &camera->GetNearClip(), 0.5f);
		ImGui::PopItemWidth();
		ImGui::Text("Far Clip");
		ImGui::PushItemWidth(120.0f);
		ImGui::DragFloat("##farclip", &camera->GetFarClip(), 0.5f);
		ImGui::PopItemWidth();
		ImGui::Spacing();
		ImGui::Spacing();
		if (ImGui::Button("Remove Component"))
		{
			ChilliEditor::s_selectedEntity->RemoveComponent(ComponentType::Camera);
		}
	}

	void EntityInspector::DrawSpriteComponentGui(const std::shared_ptr<Component> comp)
	{
	}

}
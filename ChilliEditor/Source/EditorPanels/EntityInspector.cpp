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
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text(label);
		ImGui::NextColumn();
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
		ImGui::Columns(1);
		ImGui::PopID();
	}

	void EntityInspector::DrawTransformComponentGui(const std::shared_ptr<Component> comp)
	{
		const auto& transformComp = std::static_pointer_cast<TransformComponent>(comp);
		ImGui::Spacing();
		DrawVec3Control("Translation", transformComp->Translation());
		DrawVec3Control("Rotation", transformComp->Rotation());
		DrawVec3Control("Scale", transformComp->Scale(),1.0f);
		ImGui::Spacing();
	}

	void EntityInspector::DrawScriptComponentGui(const std::shared_ptr<Component> comp)
	{
		const auto& scriptComp = std::static_pointer_cast<ScriptComponent>(comp);
		
		if (ImGui::Button("Select"))
			ImGui::OpenPopup("scriptDropdown");
		ImGui::SameLine();
		ImGui::TextUnformatted(scriptComp->GetScriptName() == "" ? "<None>" :
			scriptComp->GetScriptName().c_str());
		if (ImGui::BeginPopup("scriptDropdown"))
		{
			ImGui::Text("Scripts");
			ImGui::Separator();
			for (const auto& script : ScriptEngine::GetAvailableScripts())
			{
				if (ImGui::Selectable(script->GetScriptName().c_str()))
				{
					ScriptInstanceRepository::MakeScriptInstance(script->GetScriptName(), ChilliEditor::s_selectedEntity->Uuid.Get());
					scriptComp->SetScript(script->GetScriptName());
				}
			}
			ImGui::EndPopup();
		}
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

					if (ImGui::DragFloat(name.c_str(), &data))
					{
						scriptInstance->SetFieldValue(name, data);
					}
				}
				break;
				case FieldType::Bool:
				{
					bool data = scriptInstance->GetFieldValue<bool>(name);
					if (ImGui::Checkbox(name.c_str(), &data))
					{
						scriptInstance->SetFieldValue(name, data);
					}
				}
				break;
				case FieldType::Byte:

					break;
				case FieldType::Char:
				{

				}
				break;
				case FieldType::Double:
				{
					double data = scriptInstance->GetFieldValue<double>(name);

					if (ImGui::InputDouble(name.c_str(), &data))
					{
						scriptInstance->SetFieldValue(name, data);
					}
				}
				break;
				case FieldType::Entity:
				case FieldType::ULong:
				{

				}
				break;
				case FieldType::Int:
				{
					int data = scriptInstance->GetFieldValue<int>(name);

					if (ImGui::InputInt(name.c_str(), &data))
					{
						scriptInstance->SetFieldValue(name, data);
					}
				}
				break;
				case FieldType::Long:

					break;
				case FieldType::Short:

					break;
				case FieldType::UInt:

					break;
				case FieldType::UShort:

					break;
				case FieldType::Vector2:
				{
					DirectX::XMFLOAT2 data = scriptInstance->GetFieldValue<DirectX::XMFLOAT2>(name);
					float* vec2[2] = { &data.x,&data.y };
					if (ImGui::InputFloat2(name.c_str(), vec2[0]))
					{
						scriptInstance->SetFieldValue(name, data);
					}
				}
				break;
				case FieldType::Vector3:
				{
					DirectX::XMFLOAT3 data = scriptInstance->GetFieldValue<DirectX::XMFLOAT3>(name);
					float* vec3[3] = { &data.x,&data.y, &data.z };
					if (ImGui::InputFloat3(name.c_str(), vec3[0]))
					{
						scriptInstance->SetFieldValue(name, data);
					}
				}
				break;
				case FieldType::Vector4:
				{
					DirectX::XMFLOAT4 data = scriptInstance->GetFieldValue<DirectX::XMFLOAT4>(name);
					float* vec4[4] = { &data.x,&data.y, &data.z, &data.w };
					if (ImGui::InputFloat4(name.c_str(), vec4[0]))
					{
						scriptInstance->SetFieldValue(name, data);
					}
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
		if (ImGui::Button("Select"))
			ImGui::OpenPopup("meshDropdown");
		ImGui::SameLine();
		ImGui::TextUnformatted(!meshComp->HasMesh() ? "<None>" :
			meshComp->GetMesh()->GetName().stem().generic_string().c_str());
		if (ImGui::BeginPopup("meshDropdown"))
		{
			ImGui::Text("Meshes");
			ImGui::Separator();
			for (const auto& mesh : DependencyResolver::ResolveDependency<ProjectManager>()->GetMeshes())
			{
				if (ImGui::Selectable(mesh.second->GetName().stem().generic_string().c_str()))
				{
					meshComp->SetMesh(mesh.second->Uuid);
				}
			}
			ImGui::EndPopup();
		}
		ImGui::Spacing();
		ImGui::Text("Material");
		ImGui::ColorEdit3("Diffuse Color", &meshComp->material.diffuse.x);
		ImGui::ColorEdit3("Specular Color", &meshComp->material.specular.x);
		ImGui::InputFloat("Shininess", &meshComp->material.shininess);
		if (ImGui::Button("Select Texture"))
			ImGui::OpenPopup("textureDropdown");
		ImGui::SameLine();
		ImGui::TextUnformatted(!meshComp->HasTexture() ? "<None>" :
			meshComp->GetTexture()->GetName().stem().generic_string().c_str());
		const auto& textures = projManager->GetTextures();
		if (ImGui::BeginPopup("textureDropdown"))
		{
			ImGui::Text("Textures");
			ImGui::Separator();
			for (const auto& texture : textures)
			{
				if (ImGui::Selectable(texture.second->GetName().stem().generic_string().c_str()))
				{
					meshComp->SetTexture(texture.second->Uuid);
				}
			}
			ImGui::EndPopup();
		}
		if (ImGui::Button("Select Specular Map"))
			ImGui::OpenPopup("specularDropdown");
		ImGui::SameLine();
		ImGui::TextUnformatted(!meshComp->HasSpecularMap() ? "<None>" :
			meshComp->GetSpecularMap()->GetName().stem().generic_string().c_str());
		if (ImGui::BeginPopup("specularDropdown"))
		{
			ImGui::Text("Textures");
			ImGui::Separator();
			for (const auto& texture : textures)
			{
				if (ImGui::Selectable(texture.second->GetName().stem().generic_string().c_str()))
				{
					meshComp->SetSpecularMap(texture.second->Uuid);
				}
			}
			ImGui::EndPopup();
		}
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
		if (ImGui::BeginCombo("Body Type", currentBodyTypeSelected))
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
		ImGui::Checkbox("Fixed Rotation", rb2d->GetFixedRotation());
		ImGui::Spacing();
		if (ImGui::Button("Remove Component"))
		{
			ChilliEditor::s_selectedEntity->RemoveComponent(ComponentType::RigidBody2D);
		}
	}

	void EntityInspector::DrawCircleColliderComponentGui(const std::shared_ptr<Component> comp)
	{
		auto circleCollider = std::static_pointer_cast<CircleColliderComponent>(comp);
		float* offset[2] = { &circleCollider->GetOffset().x,&circleCollider->GetOffset().y };
		ImGui::DragFloat("Radius", &circleCollider->GetRadius(), 0.5f);
		ImGui::DragFloat2("Offset", offset[0], 0.5f);
		ImGui::DragFloat("Density", &circleCollider->GetDensity(), 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Friction", &circleCollider->GetFriction(), 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Restitution", &circleCollider->GetRestitution(), 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Restitution Threshold", &circleCollider->GetRestituitonThreshold(), 0.01f, 0.0f);
		ImGui::Spacing();
		if (ImGui::Button("Remove Component"))
		{
			ChilliEditor::s_selectedEntity->RemoveComponent(ComponentType::CircleCollider);
		}
	}

	void EntityInspector::DrawBoxColliderComponentGui(const std::shared_ptr<Component> comp)
	{
		auto bc2d = std::static_pointer_cast<BoxCollider2DComponent>(comp);
		float* offset[2] = { &bc2d->GetOffset().x,&bc2d->GetOffset().y };
		float* size[2] = { &bc2d->GetSize().x,&bc2d->GetSize().y };
		ImGui::DragFloat2("Size", size[0], 0.5f);
		ImGui::DragFloat2("Offset", offset[0], 0.5f);
		ImGui::DragFloat("Density", &bc2d->GetDensity(), 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Friction", &bc2d->GetFriction(), 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Restitution", &bc2d->GetRestitution(), 0.01f, 0.0f, 1.0f);
		ImGui::DragFloat("Restitution Threshold", &bc2d->GetRestituitonThreshold(), 0.01f, 0.0f);
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
		if (ImGui::BeginCombo("Light Type", currentLightTypeSelected))
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
		ImGui::ColorEdit3("Ambient", &light->Ambient().x);
		ImGui::ColorEdit3("Diffuse", &light->Diffuse().x);
		ImGui::ColorEdit3("Specular", &light->Specular().x);
		if (light->GetLightType() != LightType::DirectionalLight)
		{
			ImGui::Spacing();
			ImGui::Text("Attenuation");
			ImGui::DragFloat("Linear", &light->Linear(), 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Constant", &light->Constant(), 0.01f, 0.0f, 1.0f);
			ImGui::DragFloat("Quadratic", &light->Quadratic(), 0.01f, 0.0f, 1.0f);
		}
		if (light->GetLightType() == LightType::Spotlight)
		{
			ImGui::Spacing();
			ImGui::DragFloat("Inner Cut Off", &light->InnerCutOff(), 0.5f, 0.0f, 50.0f);
			ImGui::DragFloat("Outer Cut Off", &light->OuterCutOff(), 0.5f, 0.0f, 50.0f);
		}
		ImGui::Spacing();

		if (ImGui::Button("Remove Component"))
		{
			ChilliEditor::s_selectedEntity->RemoveComponent(ComponentType::Light);
		}
	}

	void EntityInspector::DrawCameraComponentGui(const std::shared_ptr<Component> comp)
	{
		auto camera = std::static_pointer_cast<CameraComponent>(comp);
		ImGui::DragFloat("Fov", &camera->GetFov(), 0.5f);
		ImGui::DragFloat("Near Clip", &camera->GetNearClip(), 0.5f);
		ImGui::DragFloat("Far Clip", &camera->GetFarClip(), 0.5f);
		const char* projectionTypeOptions[] = { "Perspective","Orthographic" };
		const char* currentProjectionTypeSelected = projectionTypeOptions[(int)camera->GetProjectionType()];
		if (ImGui::BeginCombo("Projection Type", currentProjectionTypeSelected))
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
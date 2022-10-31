#include "EntityInspector.h"
#include "../ChilliEditor.h"

namespace Chilli {

	void EntityInspector::DrawGui()
	{
		ImGui::Begin("Inspector");
		const auto& meshes = DependencyResolver::ResolveDependency<ProjectManager>()->GetMeshes();
		const auto& scripts = ScriptEngine::GetAvailableScripts();

		if (ChilliEditor::s_selectedEntity)
		{
			if (ImGui::Button("+"))
				ImGui::OpenPopup("addComponent");

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
			ImGui::Spacing();

			for (const auto& component : ChilliEditor::s_selectedEntity->GetComponents())
			{
				if (component == nullptr)
					return;

				switch (component->GetComponentType())
				{
				case ComponentType::Mesh:
				{
					const auto& projManager = DependencyResolver::ResolveDependency<ProjectManager>();
					const auto& meshComp = std::static_pointer_cast<MeshComponent>(component);
					ImGui::BeginChild("Mesh", ImVec2(0, 200), true);
					ImGui::Text("Mesh");
					if (ImGui::Button("Select"))
						ImGui::OpenPopup("meshDropdown");
					ImGui::SameLine();
					ImGui::TextUnformatted(!meshComp->HasMesh() ? "<None>" :
						meshComp->GetMesh()->GetName().stem().generic_string().c_str());
					if (ImGui::BeginPopup("meshDropdown"))
					{
						ImGui::Text("Meshes");
						ImGui::Separator();
						for (const auto& mesh : meshes)
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
					const auto textures = projManager->GetTextures();
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
					ImGui::EndChild();
				}
				break;
				case ComponentType::RigidBody2D:
				{
					const auto& rb2d = std::static_pointer_cast<RigidBody2DComponent>(component);
					ImGui::BeginChild("RigidBody 2D", ImVec2(0, 130), true);
					ImGui::Text("RigidBody 2D");
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
					ImGui::EndChild();
				}
				break;
				case ComponentType::Transform:
				{
					const auto& transformComp = std::static_pointer_cast<TransformComponent>(component);
					ImGui::BeginChild("Transform", ImVec2(0, 130), true);
					ImGui::Text("Transform");
					ImGui::Spacing();
					float* transforms[3] = { &transformComp->Translation().x,&transformComp->Translation().y, &transformComp->Translation().z };
					float* rotation[3] = { &transformComp->Rotation().x,&transformComp->Rotation().y, &transformComp->Rotation().z };
					float* scale[3] = { &transformComp->Scale().x,&transformComp->Scale().y, &transformComp->Scale().z };
					ImGui::InputFloat3("Translation", transforms[0]);
					ImGui::InputFloat3("Rotation", rotation[0]);
					ImGui::InputFloat3("Scale", scale[0]);
					ImGui::Spacing();
					ImGui::EndChild();
				}
				break;
				case ComponentType::BoxCollider2D:
				{
					auto bc2d = std::static_pointer_cast<BoxCollider2DComponent>(component);
					ImGui::BeginChild("BoxCollider 2D", ImVec2(0, 200), true);
					ImGui::Text("BoxCollider 2D");
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
					ImGui::EndChild();
				}
				break;
				case ComponentType::CircleCollider:
				{
					auto circleCollider = std::static_pointer_cast<CircleColliderComponent>(component);
					ImGui::BeginChild("Circle Collider", ImVec2(0, 200), true);
					ImGui::Text("Circle Collider");
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
					ImGui::EndChild();
				}
				break;
				case ComponentType::Camera:
				{
					auto camera = std::static_pointer_cast<CameraComponent>(component);
					ImGui::BeginChild("Camera", ImVec2(0, 200), true);
					ImGui::Text("Camera");
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
					ImGui::EndChild();
				}
				break;
				case ComponentType::Light:
				{
					auto light = std::static_pointer_cast<LightComponent>(component);
					ImGui::BeginChild("Light", ImVec2(0, 200), true);
					ImGui::Text("Light");
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
					ImGui::EndChild();
				}
				break;
				case ComponentType::Script:
				{
					const auto& scriptComp = std::static_pointer_cast<ScriptComponent>(component);
					ImGui::BeginChild("Script", ImVec2(0, 140), true);
					ImGui::Text("Script");
					if (ImGui::Button("Select"))
						ImGui::OpenPopup("scriptDropdown");
					ImGui::SameLine();
					ImGui::TextUnformatted(scriptComp->GetScriptName() == "" ? "<None>" :
						scriptComp->GetScriptName().c_str());
					if (ImGui::BeginPopup("scriptDropdown"))
					{
						ImGui::Text("Scripts");
						ImGui::Separator();
						for (const auto& script : scripts)
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
					ImGui::EndChild();
				}
				break;
				}
			}
		}
		ImGui::End();
	}
}
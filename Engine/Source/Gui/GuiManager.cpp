#include "GuiManager.h"
#include "../ResourceSystem/ProjectManager.h"
#include "../ECS/Entity.h"
#include "../Rendering/Renderer.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Chilli {

	const char* GuiManager::assetTypesList[6] = { "Meshes", "Scene", "Texture", "Scripts", "Shader", "Sounds"};
	const char* GuiManager::componentsList[18] =
	{ "Mesh","Camera","Light","Script","BoxCollider2D","RigidBody2D","CircleCollider",
		"BoxCollider","CapsuleCollider","MeshCollider","RigidBody","AudioListener","AudioSource",
		"Sprite","ParticleEmitter","Animation",	"Pathfinding","Skybox" };
	int GuiManager::assetDropdownSelected = 0;
	uint64_t GuiManager::assetFrameSelected = 0;
	uint64_t GuiManager::hierarchySelected = 0;
	uint64_t GuiManager::sceneSelected = 0;
	uint64_t GuiManager::entitySelected = 0;
	bool GuiManager::initialMousePos = true;
	float GuiManager::mouseX = 0.0f;
	float GuiManager::mouseY = 0.0f;
	float GuiManager::scenePreviewWindowWidth = 0.0f;
	float GuiManager::scenePreviewWindowHeight = 0.0f;
	std::shared_ptr<Asset> GuiManager::selectedAsset = nullptr;
	std::shared_ptr<Scene> GuiManager::selectedScene = nullptr;
	std::shared_ptr<Entity> GuiManager::selectedEntity = nullptr;

	void GuiManager::Init()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}
	}

	void GuiManager::Shutdown()
	{
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();
	}

	void GuiManager::DrawEditorGui(Renderer* renderer)
	{
		BeginFrame();
		BuildMenuBar();
		BuildAssetManager();
		BuildSceneHierarchy();
		BuildEntityInspector();
		BuildScenePreviewWindow(renderer);
		EndFrame();
	}

	void GuiManager::BeginFrame()
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	}

	void GuiManager::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void GuiManager::BuildScenePreviewWindow(Renderer* renderer)
	{
		ImGuiWindowFlags window_flags = 0;
		ImGui::Begin("Scene Preview", 0, window_flags);
		
		auto regionAvailable = ImGui::GetContentRegionAvail();
		if (scenePreviewWindowWidth != regionAvailable.x || scenePreviewWindowHeight != regionAvailable.y)
		{
			scenePreviewWindowWidth = regionAvailable.x;
			scenePreviewWindowHeight = regionAvailable.y;
			renderer->GetActiveCamera()->OnResize(regionAvailable.x, regionAvailable.y);
		}
		
		ImGui::Image(renderer->GetFrameBuffer()->GetShaderResourceView().Get(), regionAvailable);
		if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered()
			&& DependencyResolver::ResolveDependency<ProjectManager>
			()->GetCurrentScene()->GetSceneState() == SceneState::Edit)
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_None);
			if (initialMousePos)
			{
				auto winSize = ImGui::GetWindowSize();
				auto winPos = ImGui::GetWindowPos();
				mouseX = winPos.x + (winSize.x / 2);
				mouseY = winPos.y + (winSize.y / 2);
				initialMousePos = false;
			}
			else
			{
				auto mousePos = ImGui::GetMousePos();
				float xoffset = mousePos.x - mouseX;
				float yoffset = mouseY - mousePos.y;
				mouseX = mousePos.x;
				mouseY = mousePos.y;

				renderer->GetActiveCamera()->UpdateRotation(yoffset, xoffset);
			}
			if (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_W))
			{
				renderer->GetActiveCamera()->UpdatePosition(Direction::Forward);
			}

			if (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_A))
			{
				renderer->GetActiveCamera()->UpdatePosition(Direction::Left);
			}

			if (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_S))
			{
				renderer->GetActiveCamera()->UpdatePosition(Direction::Backward);
			}

			if (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_D))
			{
				renderer->GetActiveCamera()->UpdatePosition(Direction::Right);
			}
		}
		ImGui::End();
		if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			initialMousePos = true;
	}

	void GuiManager::BuildMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
				{
					selectedAsset = nullptr;
					selectedEntity = nullptr;
					selectedScene = nullptr;
					DependencyResolver::ResolveDependency<ProjectManager>()->NewProject();
				}

				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
					nfdchar_t* outPath = NULL;
					nfdresult_t result = NFD_OpenDialog("json", NULL, &outPath);
					if (result == NFD_OKAY)
					{
						DependencyResolver::ResolveDependency<ProjectManager>()->LoadProject(outPath);
						free(outPath);
						selectedAsset = nullptr;
						selectedEntity = nullptr;
						hierarchySelected = 0;
						assetDropdownSelected = 0;
						assetFrameSelected = 0;
					}
				}

				if (ImGui::MenuItem("Save", "Ctrl+S"))
				{
					nfdchar_t* outPath = NULL;
					nfdresult_t result = NFD_SaveDialog("json", NULL, &outPath);
					if (result == NFD_OKAY)
					{
						DependencyResolver::ResolveDependency<ProjectManager>()->SaveProject(outPath);
						free(outPath);
					}

				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Scene"))
			{

				auto sceneState = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene()->GetSceneState();

				if (sceneState == SceneState::Edit)
				{
					if (ImGui::MenuItem("Play"))
					{
						DependencyResolver::ResolveDependency<ProjectManager>()->PlayCurrentScene();
					}
				}

				if (sceneState == SceneState::Play)
				{
					if (ImGui::MenuItem("Stop"))
					{
						DependencyResolver::ResolveDependency<ProjectManager>()->StopCurrentScene();
					}
				}

				if (sceneState == SceneState::Edit)
				{
					if (ImGui::MenuItem("Simulate"))
					{
						DependencyResolver::ResolveDependency<ProjectManager>()->PlayCurrentScene();
					}
				}

				if (sceneState == SceneState::Play)
				{
					if (ImGui::MenuItem("Pause"))
					{
						DependencyResolver::ResolveDependency<ProjectManager>()->StopCurrentScene();
					}
				}
				ImGui::EndMenu();
			}
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
				1000.0 / float(ImGui::GetIO().Framerate), float(ImGui::GetIO().Framerate));
			ImGui::EndMainMenuBar();
		}
	}

	void GuiManager::BuildAssetManager()
	{
		ImGuiWindowFlags window_flags = 0;
		ImGui::Begin("Assets", 0, window_flags);
		ImGui::BeginGroup();
		if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
		{
			if (ImGui::BeginTabItem("Meshes"))
			{
				if (ImGui::Button("Add"))
				{
					nfdchar_t* outPath = NULL;
					nfdresult_t result = NFD_ERROR;
					result = NFD_OpenDialog("gltf,fbx", NULL, &outPath);
					if (result == NFD_OKAY)
					{
						DependencyResolver::ResolveDependency<ProjectManager>()->AddAsset(outPath, AssetType::Mesh);
						free(outPath);
					}
				}
				ImGui::Separator();
				
				const auto& meshes = DependencyResolver::ResolveDependency<ProjectManager>()->GetMeshes();
				for (const auto& mesh : meshes)
				{
					if (ImGui::Selectable(mesh.second->GetName().stem().generic_string().c_str(), assetFrameSelected == mesh.first))
					{
						selectedAsset = mesh.second;
						assetFrameSelected = selectedAsset->Uuid.Get();
					}
				}
					
				if (selectedAsset)
				{
					if (ImGui::Button("Remove Mesh"))
					{
						DependencyResolver::ResolveDependency<ProjectManager>()->RemoveAsset(selectedAsset->Uuid, AssetType::Mesh);
						selectedAsset = nullptr;
					}
				}
				
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Scenes"))
			{
				static char buffer[50] = "";
				ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer));
				if (ImGui::Button("Add Scene"))
				{
					DependencyResolver::ResolveDependency<ProjectManager>()->AddScene(buffer);
					buffer[0] = NULL;
				}
				ImGui::Separator();
				const auto& scenes = DependencyResolver::ResolveDependency<ProjectManager>()->GetScenes();
				for (const auto& scene : scenes)
				{
					if (ImGui::Selectable(scene->GetName().c_str(), sceneSelected == scene->Uuid.Get()))
					{
						selectedScene = scene;
						DependencyResolver::ResolveDependency<ProjectManager>()->SetCurrentScene(scene->Uuid);
						selectedEntity = nullptr;
						sceneSelected = selectedScene->Uuid.Get();
					}
				}
				if (selectedScene)
				{
					if (ImGui::Button("Remove Scene"))
					{
						DependencyResolver::ResolveDependency<ProjectManager>()->RemoveScene(selectedScene->Uuid);
						selectedScene = nullptr;
						selectedEntity = nullptr;
					}
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Textures"))
			{
				if (ImGui::Button("Add"))
				{
					nfdchar_t* outPath = NULL;
					nfdresult_t result = NFD_ERROR;
					result = NFD_OpenDialog("png,jpg,jpeg", NULL, &outPath);
					if (result == NFD_OKAY)
					{
						DependencyResolver::ResolveDependency<ProjectManager>()->AddAsset(outPath, AssetType::Texture);
						free(outPath);
					}
				}
				ImGui::Separator();
				const auto& textures = DependencyResolver::ResolveDependency<ProjectManager>()->GetTextures();
				for (const auto& texture : textures)
				{
					if (ImGui::Selectable(texture.second->GetName().stem().generic_string().c_str(), assetFrameSelected == texture.first))
					{
						selectedAsset = texture.second;
						assetFrameSelected = selectedAsset->Uuid.Get();
					}
				}

				if (selectedAsset)
				{
					if (ImGui::Button("Remove Texture"))
					{
						DependencyResolver::ResolveDependency<ProjectManager>()->RemoveAsset(selectedAsset->Uuid, AssetType::Texture);
						selectedAsset = nullptr;
					}
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Scripts"))
			{
				static char buffer[128] = "";
				ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer));
				if (ImGui::Button("Add Script"))
				{
					DependencyResolver::ResolveDependency<ProjectManager>()->AddAsset(buffer,AssetType::Script);
				}
				ImGui::Separator();
				const auto& scripts = ScriptEngine::GetAvailableScripts();
				for (const auto& script : scripts)
				{
					ImGui::Text(script->GetScriptName().c_str());
				}
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Shaders"))
			{
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Audio"))
			{
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem("Fonts"))
			{
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::EndGroup();
		ImGui::End();
	}

	void GuiManager::BuildSceneHierarchy()
	{
		selectedScene = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene();

		ImGuiWindowFlags window_flags = 0;

		ImGui::Begin("Scene Hierarchy", 0, window_flags);
		static char buffer[50] = "";
		ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer));
		if (ImGui::Button("Add Entity"))
		{
			selectedScene->AddEntity(buffer);
			buffer[0] = NULL;
		}
		ImGui::Separator();
		if (selectedScene)
		{
			if (ImGui::TreeNode(selectedScene->GetName().c_str()))
			{
				for (const auto& entity : selectedScene->GetEntities())
				{
					if (ImGui::Selectable(entity->GetName().c_str(), hierarchySelected == entity->Uuid.Get()))
					{
						hierarchySelected = entity->Uuid.Get();
						selectedEntity = entity;
					}
				}
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}

	void GuiManager::BuildEntityInspector()
	{
		ImGuiWindowFlags window_flags = 0;

		ImGui::Begin("Inspector", 0, window_flags);
		const auto& meshes = DependencyResolver::ResolveDependency<ProjectManager>()->GetMeshes();
		const auto& scripts = ScriptEngine::GetAvailableScripts();

		if (selectedEntity)
		{
			if (ImGui::Button("+"))
				ImGui::OpenPopup("addComponent");

			if (ImGui::BeginPopup("addComponent"))
			{
				ImGui::Text("Add Component");
				ImGui::Separator();

				for (int i = 0; i < IM_ARRAYSIZE(componentsList); i++)
				{
					if (ImGui::Selectable(componentsList[i]))
					{
						selectedEntity->AddComponent((ComponentType)i);
					}
				}
				ImGui::EndPopup();
			}
			ImGui::Spacing();

			for (const auto& component : selectedEntity->GetComponents())
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
					ImGui::ColorEdit3("Diffuse Color",&meshComp->material.diffuse.x);
					ImGui::ColorEdit3("Ambient Color", &meshComp->material.ambient.x);
					ImGui::ColorEdit3("Specular Color", &meshComp->material.specular.x);
					ImGui::InputFloat("Shininess", &meshComp->material.shininess);
					if (ImGui::Button("Select Texture"))
						ImGui::OpenPopup("textureDropdown");
					ImGui::SameLine();
					ImGui::TextUnformatted(!meshComp->HasTexture() ? "<None>" :
						meshComp->GetTexture()->GetName().stem().generic_string().c_str());
					if (ImGui::BeginPopup("textureDropdown"))
					{
						ImGui::Text("Textures");
						ImGui::Separator();
						for (const auto& texture : projManager->GetTextures())
						{
							if (ImGui::Selectable(texture.second->GetName().stem().generic_string().c_str()))
							{
								meshComp->SetTexture(texture.second->Uuid);
							}
						}
						ImGui::EndPopup();
					}
					ImGui::Spacing();
					if (ImGui::Button("Remove Component"))
					{
						selectedEntity->RemoveComponent(ComponentType::Mesh);
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
						selectedEntity->RemoveComponent(ComponentType::RigidBody2D);
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
						selectedEntity->RemoveComponent(ComponentType::RigidBody2D);
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
						selectedEntity->RemoveComponent(ComponentType::RigidBody2D);
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
						selectedEntity->RemoveComponent(ComponentType::Camera);
					}
					ImGui::EndChild();
				}
				break;
				case ComponentType::Light:
				{
					auto light = std::static_pointer_cast<LightComponent>(component);
					ImGui::BeginChild("Light", ImVec2(0, 200), true);
					ImGui::Text("Light");
					ImGui::ColorEdit3("Color", &light->Color().x);
					const char* lightTypeOptions[] = { "Directional","PointLight", "Spotlight"};
					const char* currentLightTypeSelected = lightTypeOptions[(int)light->GetLightType()];
					if (ImGui::BeginCombo("Light Type", currentLightTypeSelected))
					{
						for (int i = 0; i <= 1; i++)
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
					if (ImGui::Button("Remove Component"))
					{
						selectedEntity->RemoveComponent(ComponentType::Light);
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
								ScriptInstanceRepository::MakeScriptInstance(script->GetScriptName(), selectedEntity->Uuid.Get());
								scriptComp->SetScript(script->GetScriptName());
							}
						}
						ImGui::EndPopup();
					}
					ImGui::Spacing();
					if (scriptComp->GetScriptName() != "")
					{
						const auto& scriptInstance = ScriptInstanceRepository::GetScriptInstanceByEntityId(selectedEntity->Uuid.Get());
						auto& fields = scriptInstance->GetFields();
						for (const auto& [name,field] : fields)
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
								if (ImGui::InputFloat2(name.c_str(),vec2[0]))
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
						selectedEntity->RemoveComponent(ComponentType::Script);
					}
					ImGui::EndChild();
				}
				break;
				}
			}
		}
		ImGui::End();
	}

	bool GuiManager::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	}

	ImGuiIO* GuiManager::GetIO()
	{
		if (ImGui::GetCurrentContext())
		{
			return &ImGui::GetIO();
		}
		return nullptr;
	}
}




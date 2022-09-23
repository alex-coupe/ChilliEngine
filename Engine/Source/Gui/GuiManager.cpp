#include "GuiManager.h"
#include "../ResourceSystem/ProjectManager.h"
#include "../ECS/Entity.h"
#include "../Rendering/Renderer.h"

const char* Engine::Gui::GuiManager::assetDropdownList[4] = {"Meshes", "Audio", "Materials", "Scripts"};
const char* Engine::Gui::GuiManager::componentsList[18] = 
{ "Mesh","Camera","Light","Script","BoxCollider2D","RigidBody2D","CircleCollider",
	"BoxCollider","CapsuleCollider","MeshCollider","RigidBody","AudioListener","AudioSource",
	"Sprite","ParticleEmitter","Animation",	"Pathfinding","Skybox" };
int Engine::Gui::GuiManager::assetDropdownSelected = 0;
int Engine::Gui::GuiManager::assetFrameSelected = 0;
int Engine::Gui::GuiManager::hierarchySelected = 0;
int Engine::Gui::GuiManager::sceneSelected = 0;
int Engine::Gui::GuiManager::entitySelected = 0;
bool Engine::Gui::GuiManager::initialMousePos = true;
float Engine::Gui::GuiManager::mouseX = 0.0f;
float Engine::Gui::GuiManager::mouseY = 0.0f;
std::shared_ptr<Engine::ResourceSystem::Asset> Engine::Gui::GuiManager::selectedAsset = nullptr;
std::shared_ptr<Engine::ResourceSystem::Scene> Engine::Gui::GuiManager::selectedScene = nullptr;
std::shared_ptr<Engine::ECS::Entity> Engine::Gui::GuiManager::selectedEntity = nullptr;

void Engine::Gui::GuiManager::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

void Engine::Gui::GuiManager::Shutdown()
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();
}

void Engine::Gui::GuiManager::DrawEditorGui(Engine::Rendering::Renderer* renderer)
{
	BeginFrame();
	BuildMenuBar();
	BuildAssetManager();
	BuildSceneHierarchy();
	BuildEntityInspector();
	BuildScenePreviewWindow(renderer);
	EndFrame();
}

void Engine::Gui::GuiManager::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Engine::Gui::GuiManager::EndFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Engine::Gui::GuiManager::BuildScenePreviewWindow(Engine::Rendering::Renderer* renderer)
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	ImGui::Begin("Scene Preview", 0, window_flags);
	ImGui::Image(renderer->GetFrameBuffer()->GetShaderResourceView().Get(), ImVec2(1200, 620));
	if (ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGui::IsWindowHovered() 
		&& Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>
		()->GetCurrentScene()->GetSceneState() == Engine::ResourceSystem::SceneState::Edit)
	{
		if (initialMousePos)
		{
			auto pos = ImGui::GetMousePos();
			mouseX = pos.x;
			mouseY = pos.y;
			initialMousePos = false;
		}
		else 
		{
			auto pos = ImGui::GetMousePos();
			auto deltaX = mouseX - pos.x;
			auto deltaY = mouseY - pos.y;

			mouseX = pos.x;
			mouseY = pos.y;
			renderer->GetEditorCamera()->UpdateRotation(deltaY, deltaX);
		}
		
		if (ImGui::IsKeyDown((int)Engine::Core::Key::W))
		{
			renderer->GetEditorCamera()->UpdatePosition(DirectX::XMFLOAT3{ 0.00f,0.00f,0.01f });
		}

		if (ImGui::IsKeyDown((int)Engine::Core::Key::A))
		{
			renderer->GetEditorCamera()->UpdatePosition(DirectX::XMFLOAT3{ -0.01f,0.00f,0.0f });
		}

		if (ImGui::IsKeyDown((int)Engine::Core::Key::S))
		{
			renderer->GetEditorCamera()->UpdatePosition(DirectX::XMFLOAT3{ 0.00f,0.00f,-0.01f });
		}

		if (ImGui::IsKeyDown((int)Engine::Core::Key::D))
		{
			renderer->GetEditorCamera()->UpdatePosition(DirectX::XMFLOAT3{ 0.01f,0.00f,0.0f });
		}
				
		if (ImGui::IsKeyDown((int)Engine::Core::Key::Space))
		{
			renderer->GetEditorCamera()->UpdatePosition(DirectX::XMFLOAT3{ 0.00f,0.01f,0.0f });
		}
	}
	ImGui::End();
}

void Engine::Gui::GuiManager::BuildMenuBar()
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
				Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->NewProject();
			}

			if (ImGui::MenuItem("Open", "Ctrl+O")) 
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_OpenDialog("json", NULL, &outPath);
				if (result == NFD_OKAY) 
				{	
					Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->LoadProject(outPath);
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
					Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->SaveProject(outPath);
					free(outPath);
				}

			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Scene"))
		{

			auto sceneState = Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->GetCurrentScene()->GetSceneState();
			
			if (sceneState == Engine::ResourceSystem::SceneState::Edit)
			{
				if (ImGui::MenuItem("Play"))
				{
					Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->SetCurrentSceneState(Engine::ResourceSystem::SceneState::Play);
				}
			}

			if (sceneState == Engine::ResourceSystem::SceneState::Play)
			{
				if (ImGui::MenuItem("Stop"))
				{
					Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->SetCurrentSceneState(Engine::ResourceSystem::SceneState::Edit);
				}
			}

			if (sceneState == Engine::ResourceSystem::SceneState::Edit)
			{
				if (ImGui::MenuItem("Simulate"))
				{
					Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->SetCurrentSceneState(Engine::ResourceSystem::SceneState::Simulate);
				}
			}

			if (sceneState == Engine::ResourceSystem::SceneState::Play)
			{
				if (ImGui::MenuItem("Pause"))
				{
					Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->SetCurrentSceneState(Engine::ResourceSystem::SceneState::Pause);
				}
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void Engine::Gui::GuiManager::BuildAssetManager()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	ImGui::Begin("Assets",0, window_flags);
	ImGui::BeginGroup();
	if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Assets"))
		{
			for (int i = 0; i < IM_ARRAYSIZE(assetDropdownList); i++)
			{
				if (ImGui::Selectable(assetDropdownList[i], assetDropdownSelected == i))
				{
					assetDropdownSelected = i;
				}

			}
			if (ImGui::Button("Add"))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_ERROR;
				switch (assetDropdownSelected) {
				case (int)Engine::ResourceSystem::AssetTypes::Mesh:
					result = NFD_OpenDialog("gltf,fbx", NULL, &outPath);
					break;
				case (int)Engine::ResourceSystem::AssetTypes::Audio:
					result = NFD_OpenDialog("wav,mp3", NULL, &outPath);
					break;
				case (int)Engine::ResourceSystem::AssetTypes::Material:
					result = NFD_OpenDialog("png,jpg,jpeg,bmp", NULL, &outPath);
					break;
				case (int)Engine::ResourceSystem::AssetTypes::Script:
					result = NFD_OpenDialog("cs,dll", NULL, &outPath);
					break;
				}

				if (result == NFD_OKAY)
				{
					Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->AddAsset(outPath, (Engine::ResourceSystem::AssetTypes)assetDropdownSelected);
					free(outPath);
				}
			}
			ImGui::Separator();
			const auto& assets = Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->GetAssetsByType((Engine::ResourceSystem::AssetTypes)assetDropdownSelected);
			for (const auto& asset : assets)
			{
				if (ImGui::Selectable(asset->GetName().stem().generic_string().c_str(), assetFrameSelected == asset->GetUUID().GetUUIDHash()))
				{
					selectedAsset = asset;
					assetFrameSelected = selectedAsset->GetUUID().GetUUIDHash();
				}
			}
			if (selectedAsset)
			{
				ImGui::TextWrapped("UUID : %s", selectedAsset->GetUUID().GetUUID().c_str());
				if (ImGui::Button("Remove Asset"))
				{
					Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->RemoveAsset(selectedAsset->GetUUID());
					selectedAsset = nullptr;
				}
			}
			ImGui::EndTabItem();
		}
		
		if (ImGui::BeginTabItem("Scenes"))
		{
			static char buffer[128] = "";
			ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer));
			if (ImGui::Button("Add Scene"))
			{
				Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->AddScene(buffer);
			}
			ImGui::Separator();
			const auto& scenes = Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->GetScenes();
			for (const auto& scene : scenes)
			{
				if (ImGui::Selectable(scene->GetName().c_str(), sceneSelected == scene->GetUUID().GetUUIDHash()))
				{
					selectedScene = scene;
					Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->SetCurrentScene(scene->GetUUID());
					selectedEntity = nullptr;
					sceneSelected = selectedScene->GetUUID().GetUUIDHash();
				}
			}
			if (selectedScene)
			{
				ImGui::TextWrapped("UUID : %s", selectedScene->GetUUID().GetUUID().c_str());
				if (ImGui::Button("Remove Scene"))
				{
					Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->RemoveScene(selectedScene->GetUUID());
					selectedScene = nullptr;
					selectedEntity = nullptr;
				}
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Entities"))
		{
			if (selectedScene)
			{
				static char buffer[128] = "";
				ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer));
				if (ImGui::Button("Add Entity"))
				{
					selectedScene->AddEntity(buffer);
				}

				const auto& entities = selectedScene->GetEntities();
				for (const auto& entity : entities)
				{
					if (ImGui::Selectable(entity->GetName().c_str(), entitySelected == entity->GetUUID().GetUUIDHash()))
					{
						selectedEntity = entity;
						entitySelected = selectedEntity->GetUUID().GetUUIDHash();
					}

				}
				if (selectedEntity && selectedScene)
				{
					ImGui::TextWrapped("UUID : %s", selectedEntity->GetUUID().GetUUID().c_str());
					if (ImGui::Button("Remove Entity"))
					{
						selectedScene->RemoveEntity(selectedEntity->GetUUID());
						selectedEntity = nullptr;
					}
				}

			}
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::EndGroup();
	ImGui::End();
}

void Engine::Gui::GuiManager::BuildSceneHierarchy()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	ImGui::Begin("Scene Hierarchy", 0, window_flags);
	const auto& scenes = Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->GetScenes();		
	for (const auto& scene : scenes)
	{
		if (ImGui::TreeNode(scene->GetName().c_str()))
		{
			for (const auto& entity : scene->GetEntities()) 
			{
				if (ImGui::Selectable(entity->GetName().c_str(), hierarchySelected == entity->GetUUID().GetUUIDHash()))
				{
					hierarchySelected = entity->GetUUID().GetUUIDHash();
					selectedEntity = entity;
				}
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void Engine::Gui::GuiManager::BuildEntityInspector()
{
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	ImGui::Begin("Inspector",0,window_flags);
	const auto& meshes = Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->GetAssetsByType(Engine::ResourceSystem::AssetTypes::Mesh);
	const auto& scripts = Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->GetAvailableScripts();

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
					selectedEntity->AddComponent((Engine::ECS::ComponentTypes)i);					
				}
			}
			ImGui::EndPopup();
		}
		ImGui::Spacing();

		for (const auto& component : selectedEntity->GetComponents())
		{
			switch (component->GetComponentType())
			{
				case Engine::ECS::ComponentTypes::Mesh:
				{
					const auto& meshComp = std::static_pointer_cast<Engine::ECS::MeshComponent>(component);
					ImGui::BeginChild("Mesh", ImVec2(0, 140), true);
					ImGui::Text("Mesh");
					if (ImGui::Button("Select"))
						ImGui::OpenPopup("meshDropdown");
					ImGui::SameLine();
					ImGui::TextUnformatted(!meshComp->GetMesh() ? "<None>" :
						meshComp->GetMesh()->GetName().stem().generic_string().c_str());
					if (ImGui::BeginPopup("meshDropdown"))
					{
						ImGui::Text("Meshes");
						ImGui::Separator();
						for (const auto& mesh : meshes)
						{
							if (ImGui::Selectable(mesh->GetName().stem().generic_string().c_str()))
							{
								meshComp->SetMesh(std::static_pointer_cast<Engine::ResourceSystem::Mesh>(mesh));
							}
						}
						ImGui::EndPopup();
					}
					ImGui::Spacing();
					ImGui::Text("Material");
					ImGui::Spacing();
					if (ImGui::Button("Remove Component"))
					{
						selectedEntity->RemoveComponent(Engine::ECS::ComponentTypes::Mesh);
					}
					ImGui::EndChild();
				}
					break;
				case Engine::ECS::ComponentTypes::RigidBody2D:
				{
					const auto& rb2d = std::static_pointer_cast<Engine::ECS::RigidBody2DComponent>(component);
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
								rb2d->SetBodyType((Engine::ECS::BodyType)i);
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
						selectedEntity->RemoveComponent(Engine::ECS::ComponentTypes::RigidBody2D);
					}
					ImGui::EndChild();
				}
					break;
				case Engine::ECS::ComponentTypes::Transform:
				{
					const auto& transformComp = std::static_pointer_cast<Engine::ECS::TransformComponent>(component);
					ImGui::BeginChild("Transform", ImVec2(0, 130), true);
					ImGui::Text("Transform");
					ImGui::Spacing();
					float* transforms[3] = { &transformComp->GetTranslation().x,&transformComp->GetTranslation().y, &transformComp->GetTranslation().z };
					float* rotation[3] = { &transformComp->GetRotation().x,&transformComp->GetRotation().y, &transformComp->GetRotation().z };
					float* scale[3] = { &transformComp->GetScale().x,&transformComp->GetScale().y, &transformComp->GetScale().z };
					ImGui::InputFloat3("Translation", transforms[0]);
					ImGui::InputFloat3("Rotation", rotation[0]);
					ImGui::InputFloat3("Scale", scale[0]);
					ImGui::Spacing();
					ImGui::EndChild();
				}
					break;
				case Engine::ECS::ComponentTypes::BoxCollider2D:
				{
					auto bc2d = std::static_pointer_cast<Engine::ECS::BoxCollider2DComponent>(component);
					ImGui::BeginChild("BoxCollider 2D", ImVec2(0, 200), true);
					ImGui::Text("BoxCollider 2D");
					float* offset[2] = { &bc2d->GetOffset().x,&bc2d->GetOffset().y};
					float* size[2] = { &bc2d->GetSize().x,&bc2d->GetSize().y };
					ImGui::DragFloat2("Size", size[0],0.5f);
					ImGui::DragFloat2("Offset", offset[0],0.5f);
					ImGui::DragFloat("Density", &bc2d->GetDensity(),0.01f,0.0f,1.0f);
					ImGui::DragFloat("Friction", &bc2d->GetFriction(), 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("Restitution", &bc2d->GetRestitution(), 0.01f, 0.0f, 1.0f);
					ImGui::DragFloat("Restitution Threshold", &bc2d->GetRestituitonThreshold(), 0.01f, 0.0f);
					ImGui::Spacing();
					if (ImGui::Button("Remove Component"))
					{
						selectedEntity->RemoveComponent(Engine::ECS::ComponentTypes::RigidBody2D);
					}
					ImGui::EndChild();
				}
				break;
				case Engine::ECS::ComponentTypes::CircleCollider:
				{
					auto circleCollider = std::static_pointer_cast<Engine::ECS::CircleColliderComponent>(component);
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
						selectedEntity->RemoveComponent(Engine::ECS::ComponentTypes::RigidBody2D);
					}
					ImGui::EndChild();
				}
				break;
				case Engine::ECS::ComponentTypes::Script:
				{
					const auto& scriptComp = std::static_pointer_cast<Engine::ECS::ScriptComponent>(component);
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
							if (ImGui::Selectable(script.c_str()))
							{
								scriptComp->SetScript(script);
							}
						}
						ImGui::EndPopup();
					}
					ImGui::Spacing();
					ImGui::Text("Material");
					ImGui::Spacing();
					if (ImGui::Button("Remove Component"))
					{
						selectedEntity->RemoveComponent(Engine::ECS::ComponentTypes::Mesh);
					}
					ImGui::EndChild();
				}
				break;
			}
		}
	}
	ImGui::End();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

bool Engine::Gui::GuiManager::WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
}

ImGuiIO* Engine::Gui::GuiManager::GetIO()
{
	if (ImGui::GetCurrentContext())
	{
		return &ImGui::GetIO();
	}
	return nullptr;
}




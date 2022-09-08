#include "GuiManager.h"
#include "../ResourceSystem/ProjectManager.h"
#include "../ECS/Entity.h"

const char* Engine::Gui::GuiManager::assetDropdownList[4] = {"Meshes", "Audio", "Materials", "Scripts"};
const char* Engine::Gui::GuiManager::componentsList[5] = 
{ "Mesh", "2D Collider", "Camera", "Point Light", "Script"};
int Engine::Gui::GuiManager::assetDropdownSelected = 0;
int Engine::Gui::GuiManager::assetFrameSelected = 0;
int Engine::Gui::GuiManager::hierarchySelected = 0;
std::shared_ptr<Engine::ResourceSystem::Asset> Engine::Gui::GuiManager::selectedAsset = nullptr;
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

void Engine::Gui::GuiManager::DrawEditorGui()
{
	BeginFrame();
	BuildMenuBar();
	BuildAssetManager();
	BuildSceneHierarchy();
	BuildEntityInspector();
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

void Engine::Gui::GuiManager::BuildMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New")) 
			{
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
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
			if (ImGui::MenuItem("Redo", "CTRL+Y")) {} 
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "CTRL+X")) {}
			if (ImGui::MenuItem("Copy", "CTRL+C")) {}
			if (ImGui::MenuItem("Paste", "CTRL+V")) {}
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
	ImGui::BeginChild("left pane", ImVec2(300, 0), true);
	ImGui::Combo("", &assetDropdownSelected, assetDropdownList, IM_ARRAYSIZE(assetDropdownList));
	if (ImGui::Button("Add"))
	{
		nfdchar_t* outPath = NULL;
		nfdresult_t result = NFD_ERROR;
		switch (assetDropdownSelected) {
		case 0:
			result = NFD_OpenDialog("gltf,fbx", NULL, &outPath);
			break;
		case 1:
			result = NFD_OpenDialog("wav,mp3", NULL, &outPath);
			break;
		case 2:
			result = NFD_OpenDialog("png,jpg,jpeg,bmp", NULL, &outPath);
			break;
		case 3:
			result = NFD_OpenDialog("cs", NULL, &outPath);
			break;
		}
		
		if (result == NFD_OKAY)
		{
			Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->AddAsset(outPath,(Engine::ResourceSystem::AssetTypes)assetDropdownSelected);
			free(outPath);
		}
	}
	const auto assets = Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->GetAssetsByType((Engine::ResourceSystem::AssetTypes)assetDropdownSelected);
	for (const auto& asset : assets)
	{
		if (ImGui::Selectable(asset->GetName().stem().generic_string().c_str(), assetFrameSelected == asset->GetUUID().GetUUIDHash()))
		{
			selectedAsset = asset;
			assetFrameSelected = selectedAsset->GetUUID().GetUUIDHash();
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginGroup();
	ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
	
	ImGui::Separator();
	if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Description") && selectedAsset != nullptr)
		{
			ImGui::TextWrapped("UUID : %s",selectedAsset->GetUUID().GetUUID().c_str());
		}
		ImGui::EndTabItem();
		ImGui::EndTabBar();
	}
	ImGui::EndChild();
	if (ImGui::Button("Remove Asset")) 
	{
		Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>()->RemoveAsset(selectedAsset->GetUUID());
	}
	ImGui::SameLine();
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
	std::shared_ptr<Engine::ECS::MeshComponent> meshComp = nullptr;
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
					meshComp = std::static_pointer_cast<Engine::ECS::MeshComponent>(component);
					ImGui::BeginChild("Mesh Component", ImVec2(0, 140), true);
					ImGui::Text("Mesh Component");
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
					break;
				case Engine::ECS::ComponentTypes::Transform:
					const auto& transformComp = std::static_pointer_cast<Engine::ECS::TransformComponent>(component);
					ImGui::BeginChild("Transform Component", ImVec2(0, 130), true);
					ImGui::Text("Transform Component");
					ImGui::Spacing();
					float* transforms[3] = { &transformComp->GetTranslation().x,&transformComp->GetTranslation().y, &transformComp->GetTranslation().z };
					float* rotation[3] = { &transformComp->GetRotation().x,&transformComp->GetRotation().y, &transformComp->GetRotation().z };
					float* scale[3] = { &transformComp->GetScale().x,&transformComp->GetScale().y, &transformComp->GetScale().z };
					ImGui::InputFloat3("Translation", transforms[0]);
					ImGui::InputFloat3("Rotation", rotation[0]);
					ImGui::InputFloat3("Scale", scale[0]);
					ImGui::Spacing();
					ImGui::EndChild();
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




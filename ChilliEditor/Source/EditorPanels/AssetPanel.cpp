#include "AssetPanel.h"
#include "../ChilliEditor.h"

Chilli::AssetPanel::AssetPanel()
{
	ChilliEditor::s_selectedScene = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene();
}

void Chilli::AssetPanel::DrawGui()
{
	ImGui::Begin("Assets", 0);
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
				if (ImGui::Selectable(mesh.second->GetName().stem().generic_string().c_str(), ChilliEditor::s_selectedAsset && ChilliEditor::s_selectedAsset->Uuid.Get() == mesh.first))
				{
					ChilliEditor::s_selectedAsset = mesh.second;
				}
			}

			if (ChilliEditor::s_selectedAsset)
			{
				if (ImGui::Button("Remove Mesh"))
				{
					DependencyResolver::ResolveDependency<ProjectManager>()->RemoveAsset(ChilliEditor::s_selectedAsset->Uuid, AssetType::Mesh);
					ChilliEditor::s_selectedAsset = nullptr;
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
				if (ImGui::Selectable(scene->GetName().c_str(), ChilliEditor::s_selectedScene->Uuid.Get() == scene->Uuid.Get()))
				{
					ChilliEditor::s_selectedScene = scene;
					DependencyResolver::ResolveDependency<ProjectManager>()->SetCurrentScene(scene->Uuid);
					ChilliEditor::s_selectedEntity = nullptr;
				}
			}
			if (ChilliEditor::s_selectedScene)
			{
				if (ImGui::Button("Remove Scene"))
				{
					DependencyResolver::ResolveDependency<ProjectManager>()->RemoveScene(ChilliEditor::s_selectedScene->Uuid);
					ChilliEditor::s_selectedScene = nullptr;
					ChilliEditor::s_selectedEntity = nullptr;
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
				if (ImGui::Selectable(texture.second->GetName().stem().generic_string().c_str(), ChilliEditor::s_selectedAsset && ChilliEditor::s_selectedAsset->Uuid.Get() == texture.first))
				{
					ChilliEditor::s_selectedAsset = texture.second;
				}
			}

			if (ChilliEditor::s_selectedAsset)
			{
				if (ImGui::Button("Remove Texture"))
				{
					DependencyResolver::ResolveDependency<ProjectManager>()->RemoveAsset(ChilliEditor::s_selectedAsset->Uuid, AssetType::Texture);
					ChilliEditor::s_selectedAsset = nullptr;
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
				DependencyResolver::ResolveDependency<ProjectManager>()->AddAsset(buffer, AssetType::Script);
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

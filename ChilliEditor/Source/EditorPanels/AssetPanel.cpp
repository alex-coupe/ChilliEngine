#include "AssetPanel.h"
#include "../ChilliEditor.h"

Chilli::AssetPanel::AssetPanel()
{
	m_fileTexture = std::make_unique<Texture>("Resources/Icons/FileIcon.png");
	m_folderTexture = std::make_unique<Texture>("Resources/Icons/DirectoryIcon.png");
	ChilliEditor::s_selectedScene = DependencyResolver::ResolveDependency<ProjectManager>()->GetCurrentScene();
}

void Chilli::AssetPanel::DrawGui()
{
	auto projMan = DependencyResolver::ResolveDependency<ProjectManager>();
	ImGui::Begin("Assets", 0);
	ImGui::BeginGroup();
	if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
	{
		if (ImGui::BeginTabItem("Meshes"))
		{
			static float padding = 8.0f;
			static float thumbnailSize = 64.0f;
			
			float cellSize = thumbnailSize + padding;
			
			float panelWidth = ImGui::GetContentRegionAvail().x;
			
			int columnCount = (int)(panelWidth / cellSize);
			
			if (columnCount < 1)
				columnCount = 1;

			ImGui::Columns(columnCount, 0, false);
			for (const auto& mesh : projMan->GetMeshes())
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				if (ImGui::ImageButtonEx((int)mesh.first, m_fileTexture->GetShaderResourceView().Get()
					,{ thumbnailSize,thumbnailSize }, { 0,1 }, {1,0},ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
				{
					ImGui::OpenPopup("removemesh");
				}

				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("CONTENT_BROWSER_MESH", &mesh.first, sizeof(uint64_t));
					ImGui::EndDragDropSource();
				}

				ImGui::TextWrapped(mesh.second->GetName().stem().string().c_str());
				ImGui::PopStyleColor();
				ImGui::NextColumn();
			}
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (ImGui::ImageButton(m_folderTexture->GetShaderResourceView().Get(),{ thumbnailSize,thumbnailSize }, { 1,0 }, { 0,1 }))
			{
				nfdchar_t* outPath = NULL;
				nfdresult_t result = NFD_ERROR;
				result = NFD_OpenDialog("gltf,fbx", NULL, &outPath);
				if (result == NFD_OKAY)
				{
					projMan->AddAsset(outPath, AssetType::Mesh);
					free(outPath);
				}
			}
			ImGui::PopStyleColor();
			ImGui::Columns(1);			

			if (ImGui::BeginPopup("removemesh"))
			{
				if (ImGui::Button("Remove Mesh"))
				{
					projMan->RemoveAsset(ChilliEditor::s_selectedAsset->Uuid, AssetType::Mesh);
					ChilliEditor::s_selectedAsset = nullptr;
				}
				ImGui::EndPopup();
			}
			
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Scenes"))
		{
			static char buffer[50] = "";
			ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer));
			if (ImGui::Button("Add Scene"))
			{
				projMan->AddScene(buffer);
				buffer[0] = NULL;
			}
			ImGui::Separator();
			const auto& scenes = projMan->GetScenes();
			for (const auto& scene : scenes)
			{
				if (ImGui::Selectable(scene->GetName().c_str(), ChilliEditor::s_selectedScene->Uuid.Get() == scene->Uuid.Get()))
				{
					ChilliEditor::s_selectedScene = scene;
					projMan->SetCurrentScene(scene->Uuid);
					ChilliEditor::s_selectedEntity = nullptr;
				}
			}
			if (ChilliEditor::s_selectedScene)
			{
				if (ImGui::Button("Remove Scene"))
				{
					projMan->RemoveScene(ChilliEditor::s_selectedScene->Uuid);
					ChilliEditor::s_selectedScene = nullptr;
					ChilliEditor::s_selectedEntity = nullptr;
				}
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Materials"))
		{
			if (ImGui::BeginPopupContextItem("new_material"))
			{
				ImGui::Text("New Material");
				static char nameBuff[256];
				static Material mat = {};
				ImGui::InputText("Name", nameBuff, IM_ARRAYSIZE(nameBuff));
				ImGui::ColorEdit3("Diffuse Color", mat.DiffuseColor);
				ImGui::ColorEdit3("Specular Color", mat.SpecularColor);
				ImGui::InputFloat("Shininess", &mat.Shininess);
				if (ImGui::Button("Select Texture"))
					ImGui::OpenPopup("textureDropdown");
				ImGui::SameLine();
				ImGui::TextUnformatted(mat.DiffuseTexId == 0 ? "<None>" :
					std::static_pointer_cast<Texture>(projMan->GetAssetByUUID(mat.DiffuseTexId, AssetType::Texture))->GetName().stem().generic_string().c_str());
				const auto& textures = projMan->GetTextures();
				if (ImGui::BeginPopup("textureDropdown"))
				{
					ImGui::Text("Textures");
					ImGui::Separator();
					for (const auto& texture : textures)
					{
						if (ImGui::Selectable(texture.second->GetName().stem().generic_string().c_str()))
						{
							mat.DiffuseTexId = texture.second->Uuid.Get();
						}
					}
					ImGui::EndPopup();
				}
				if (ImGui::Button("Select Specular Map"))
					ImGui::OpenPopup("specularDropdown");
				ImGui::SameLine();
				ImGui::TextUnformatted(mat.SpecularTexId == 0 ? "<None>" :
					std::static_pointer_cast<Texture>(projMan->GetAssetByUUID(mat.SpecularTexId, AssetType::Texture))->GetName().stem().generic_string().c_str());
				if (ImGui::BeginPopup("specularDropdown"))
				{
					ImGui::Text("Textures");
					ImGui::Separator();
					for (const auto& texture : textures)
					{
						if (ImGui::Selectable(texture.second->GetName().stem().generic_string().c_str()))
						{
							mat.SpecularTexId = texture.second->Uuid.Get();
						}
					}
					ImGui::EndPopup();
				}
				if (ImGui::Button("Ok"))
				{
					mat.Name = nameBuff;
					projMan->CreateMaterial(mat);
					mat = {};
					mat.Name = "";
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
				{
					mat = {};
					mat.Name = "";
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
			if (ImGui::Button("New"))
				ImGui::OpenPopup("new_material");

			ImGui::Separator();
			for (const auto& material : projMan->GetMaterials())
			{
				if (ImGui::Selectable(material.second.Name.c_str()))
				{
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
					projMan->AddAsset(outPath, AssetType::Texture);
					free(outPath);
				}
			}
			ImGui::Separator();
			const auto& textures = projMan->GetTextures();
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
					projMan->RemoveAsset(ChilliEditor::s_selectedAsset->Uuid, AssetType::Texture);
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
				projMan->AddAsset(buffer, AssetType::Script);
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

#include "AssetPanel.h"
#include "../ChilliEditor.h"

Chilli::AssetPanel::AssetPanel()
{
	m_fileTexture = std::make_unique<Texture>("Resources/Icons/FileIcon.png");
	m_folderTexture = std::make_unique<Texture>("Resources/Icons/DirectoryIcon.png");
	m_plusIcon = std::make_unique<Texture>("Resources/Icons/PlusIcon.png");
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
					ChilliEditor::s_selectedAsset = mesh.second;
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
			static float padding = 8.0f;
			static float thumbnailSize = 64.0f;

			float cellSize = thumbnailSize + padding;

			float panelWidth = ImGui::GetContentRegionAvail().x;

			int columnCount = (int)(panelWidth / cellSize);

			if (columnCount < 1)
				columnCount = 1;

			ImGui::Columns(columnCount, 0, false);

			const auto& scenes = projMan->GetScenes();
			static uint64_t sceneToEdit = 0;
			for (const auto& scene : scenes)
			{
				ChilliEditor::s_selectedScene->Uuid == scene->Uuid ? ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.305f, 0.31f, 1.0f)) : ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				
				if (ImGui::ImageButtonEx((int)scene->Uuid.Get(), m_fileTexture->GetShaderResourceView().Get()
					, { thumbnailSize,thumbnailSize }, { 0,1 }, { 1,0 }, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
				{
					ChilliEditor::s_selectedScene = scene;
				}
				if (ImGui::IsItemClicked(ImGuiMouseButton_Right) && ChilliEditor::s_selectedScene->Uuid != scene->Uuid)
				{
					sceneToEdit = scene->Uuid.Get();
					ImGui::OpenPopup("editscene");
				}

				if (ImGui::BeginDragDropSource())
				{
					auto id = scene->Uuid.Get();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_SCENE", &id, sizeof(uint64_t));
					ImGui::EndDragDropSource();
				}

				ImGui::TextWrapped(scene->GetName().c_str());
				ImGui::PopStyleColor();
				ImGui::NextColumn();
			}
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (ImGui::ImageButton(m_plusIcon->GetShaderResourceView().Get(), { thumbnailSize,thumbnailSize }, { 1,0 }, { 0,1 }))
				ImGui::OpenPopup("addscene");
	
			ImGui::PopStyleColor();
			ImGui::Columns(1);

			if (ImGui::BeginPopup("addscene"))
			{
				static char buffer[256] = "";
				ImGui::Text("Name");
				ImGui::Spacing();
				ImGui::InputText("##name", buffer, IM_ARRAYSIZE(buffer));
				ImGui::Spacing();
				if (ImGui::Button("Add"))
				{
					projMan->AddScene(buffer);
					buffer[0] = NULL;
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
				{
					buffer[0] = NULL;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			if (ImGui::BeginPopup("editscene") && sceneToEdit != 0)
			{
				static char sceneNameBuffer[256] = "";
				auto scene = projMan->GetScene(sceneToEdit);
				strcpy_s(sceneNameBuffer, scene->GetName().c_str());
				ImGui::Text("Name");
				ImGui::InputText("##name", sceneNameBuffer, IM_ARRAYSIZE(sceneNameBuffer));
				if (ImGui::Button("Remove Scene"))
				{
					projMan->RemoveScene(sceneToEdit);
					sceneToEdit = 0;
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Close"))
				{
					scene->SetName(sceneNameBuffer);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Materials"))
		{
			static float padding = 8.0f;
			static float thumbnailSize = 64.0f;

			float cellSize = thumbnailSize + padding;

			float panelWidth = ImGui::GetContentRegionAvail().x;

			int columnCount = (int)(panelWidth / cellSize);

			if (columnCount < 1)
				columnCount = 1;

			static uint64_t materialId = 0;

			ImGui::Columns(columnCount, 0, false);
			for (const auto& material : projMan->GetMaterials())
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::PushID(material.second.Id.Get());
				if (ImGui::ImageButton(m_fileTexture->GetShaderResourceView().Get(), { thumbnailSize, thumbnailSize }, { 1,0 }, { 0,1 }))
				{
					materialId = material.first;
					ImGui::OpenPopup("editmaterial");
				}
				ImGui::PopID();

				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("CONTENT_BROWSER_MATERIAL", &material.first, sizeof(uint64_t));
					ImGui::EndDragDropSource();
				}

				ImGui::TextWrapped(material.second.Name.c_str());
				ImGui::PopStyleColor();
				ImGui::NextColumn();
			}
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (ImGui::ImageButton(m_folderTexture->GetShaderResourceView().Get(), { thumbnailSize,thumbnailSize }, { 1,0 }, { 0,1 }))
			{
				materialId = 0;
				ImGui::OpenPopup("newmaterial");
			}
			ImGui::PopStyleColor();
			ImGui::Columns(1);
			
			if (ImGui::BeginPopupContextItem("editmaterial"))
			{
				static auto& mat = projMan->GetMaterial(materialId);
				static char nameBuff[256];
				ImGui::Text("Edit Material");
				ImGui::Spacing();
				strcpy_s(nameBuff, mat.Name.c_str());
				ImGui::Text("Name");
				ImGui::InputText("##name", nameBuff, IM_ARRAYSIZE(nameBuff));
				ImGui::Spacing();
				ImGui::Text("Diffuse");
				ImGui::ColorEdit3("##diffusecolor", mat.DiffuseColor);
				ImGui::Text("Specular");
				ImGui::ColorEdit3("##specular", mat.SpecularColor);
				ImGui::PushItemWidth(120.0f);
				ImGui::Text("Shininess");
				ImGui::InputFloat("##shininess", &mat.Shininess);
				ImGui::PopItemWidth();

				const auto& textures = projMan->GetTextures();

				ImGui::Spacing();
				ImGui::Text("Diffuse Texture");
				ImGui::PushItemWidth(180.0f);
				if (ImGui::BeginCombo("##difftex", mat.DiffuseTexId == 0 ? "None" : projMan->GetAssetByUUID(mat.DiffuseTexId, AssetType::Texture)->GetName().stem().string().c_str()))
				{
					bool noneSelected = mat.DiffuseTexId == 0;
					if (ImGui::Checkbox("None", &noneSelected))
					{
						mat.DiffuseTexId = 0;
					}

					for (const auto& texture : textures)
					{
						bool selected = texture.first == mat.DiffuseTexId;
						if (ImGui::Checkbox(texture.second->GetName().stem().string().c_str(), &selected))
						{
							mat.DiffuseTexId = texture.first;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::PopItemWidth();

				ImGui::Spacing();
				ImGui::Text("Specular Texture");
				ImGui::PushItemWidth(180.0f);
				if (ImGui::BeginCombo("##spectex", mat.SpecularTexId == 0 ? "None" : projMan->GetAssetByUUID(mat.SpecularTexId, AssetType::Texture)->GetName().stem().string().c_str()))
				{
					bool noneSelected = mat.SpecularTexId == 0;
					if (ImGui::Checkbox("None", &noneSelected))
					{
						mat.SpecularTexId = 0;
					}

					for (const auto& texture : textures)
					{
						bool selected = texture.first == mat.SpecularTexId;
						if (ImGui::Checkbox(texture.second->GetName().stem().string().c_str(), &selected))
						{
							mat.SpecularTexId = texture.first;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::Spacing();
				ImGui::Spacing();
				if (materialId != 1 && materialId != 0)
				{
					if (ImGui::Button("Delete Material"))
					{
						projMan->RemoveMaterial(materialId);
						materialId = 0;
						ImGui::CloseCurrentPopup();
					}
					ImGui::SameLine();
				}
				
				ImGui::SameLine();
				if (ImGui::Button("Close"))
				{
					materialId = 0;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

			if (ImGui::BeginPopupContextItem("newmaterial"))
			{
				static Material mat = {};
				static char nameBuff[256];
				ImGui::Text("New Material");
				ImGui::Spacing();
				ImGui::Text("Name");
				ImGui::InputText("##name", nameBuff, IM_ARRAYSIZE(nameBuff));
				ImGui::Spacing();
				ImGui::Text("Diffuse");
				ImGui::ColorEdit3("##diffusecolor", mat.DiffuseColor);
				ImGui::Text("Specular");
				ImGui::ColorEdit3("##specular", mat.SpecularColor);
				ImGui::PushItemWidth(120.0f);
				ImGui::Text("Shininess");
				ImGui::InputFloat("##shininess", &mat.Shininess);
				ImGui::PopItemWidth();

				const auto& textures = projMan->GetTextures();

				ImGui::Spacing();
				ImGui::Text("Diffuse Texture");
				ImGui::PushItemWidth(180.0f);
				if (ImGui::BeginCombo("##difftex", mat.DiffuseTexId == 0 ? "None" : projMan->GetAssetByUUID(mat.DiffuseTexId, AssetType::Texture)->GetName().stem().string().c_str()))
				{
					bool noneSelected = mat.DiffuseTexId == 0;
					if (ImGui::Checkbox("None", &noneSelected))
					{
						mat.DiffuseTexId = 0;
					}

					for (const auto& texture : textures)
					{
						bool selected = texture.first == mat.DiffuseTexId;
						if (ImGui::Checkbox(texture.second->GetName().stem().string().c_str(), &selected))
						{
							mat.DiffuseTexId = texture.first;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::PopItemWidth();

				ImGui::Spacing();
				ImGui::Text("Specular Texture");
				ImGui::PushItemWidth(180.0f);
				if (ImGui::BeginCombo("##spectex", mat.SpecularTexId == 0 ? "None" : projMan->GetAssetByUUID(mat.SpecularTexId, AssetType::Texture)->GetName().stem().string().c_str()))
				{
					bool noneSelected = mat.SpecularTexId == 0;
					if (ImGui::Checkbox("None", &noneSelected))
					{
						mat.SpecularTexId = 0;
					}

					for (const auto& texture : textures)
					{
						bool selected = texture.first == mat.SpecularTexId;
						if (ImGui::Checkbox(texture.second->GetName().stem().string().c_str(), &selected))
						{
							mat.SpecularTexId = texture.first;
						}
					}
					ImGui::EndCombo();
				}
				ImGui::PopItemWidth();
				ImGui::Spacing();
				ImGui::Spacing();
				if (ImGui::Button("Ok"))
				{
					mat.Name = nameBuff;
					projMan->CreateMaterial(mat);
					materialId = 0;
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Close"))
				{
					materialId = 0;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Textures"))
		{
			static float padding = 8.0f;
			static float thumbnailSize = 64.0f;

			float cellSize = thumbnailSize + padding;

			float panelWidth = ImGui::GetContentRegionAvail().x;

			int columnCount = (int)(panelWidth / cellSize);

			if (columnCount < 1)
				columnCount = 1;

			ImGui::Columns(columnCount, 0, false);
			
			const auto& textures = projMan->GetTextures();
			for (const auto& texture : textures)
			{
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				if (ImGui::ImageButtonEx((int)texture.first, m_fileTexture->GetShaderResourceView().Get()
					, { thumbnailSize,thumbnailSize }, { 0,1 }, { 1,0 }, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1, 1, 1, 1)))
				{
					ChilliEditor::s_selectedAsset = texture.second;
					ImGui::OpenPopup("removetexture");
				}

				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("CONTENT_BROWSER_TEXTURE", &texture.first, sizeof(uint64_t));
					ImGui::EndDragDropSource();
				}

				ImGui::TextWrapped(texture.second->GetName().stem().string().c_str());
				ImGui::PopStyleColor();
				ImGui::NextColumn();
			}
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (ImGui::ImageButton(m_folderTexture->GetShaderResourceView().Get(), { thumbnailSize,thumbnailSize }, { 1,0 }, { 0,1 }))
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
			ImGui::PopStyleColor();
			ImGui::Columns(1);

			if (ImGui::BeginPopup("removetexture"))
			{
				if (ImGui::Button("Remove Texture"))
				{
					projMan->RemoveAsset(ChilliEditor::s_selectedAsset->Uuid, AssetType::Texture);
					ChilliEditor::s_selectedAsset = nullptr;
				}
				ImGui::EndPopup();
			}

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Scripts"))
		{
			static float padding = 8.0f;
			static float thumbnailSize = 64.0f;

			float cellSize = thumbnailSize + padding;

			float panelWidth = ImGui::GetContentRegionAvail().x;

			int columnCount = (int)(panelWidth / cellSize);

			if (columnCount < 1)
				columnCount = 1;

			ImGui::Columns(columnCount, 0, false);
			const auto& scripts = ScriptEngine::GetAvailableScripts();
			for (const auto& script : scripts)
			{
				ImGui::PushID(script->GetScriptName().c_str());
				ImGui::Image(m_fileTexture->GetShaderResourceView().Get(), { thumbnailSize,thumbnailSize });
			
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
				{
					ImGui::SetDragDropPayload("CONTENT_BROWSER_SCRIPT", script->GetScriptName().c_str(), strlen(script->GetScriptName().c_str()) * sizeof(char));
					ImGui::EndDragDropSource();
				}
				ImGui::PopID();

				ImGui::TextWrapped(script->GetScriptName().c_str());
				ImGui::NextColumn();
			}
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
			if (ImGui::ImageButton(m_folderTexture->GetShaderResourceView().Get(), { thumbnailSize,thumbnailSize }, { 1,0 }, { 0,1 }))
			{
				ImGui::OpenPopup("addscript");
			}
			ImGui::PopStyleColor();
			ImGui::Columns(1);

			if (ImGui::BeginPopup("addscript"))
			{
				static char buffer[128] = "";
				ImGui::InputText("Name", buffer, IM_ARRAYSIZE(buffer));
				if (ImGui::Button("Add Script"))
				{
					projMan->AddAsset(buffer, AssetType::Script);
					buffer[0] = NULL;
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("Cancel"))
				{
					buffer[0] = NULL;
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
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
		if (ImGui::BeginTabItem("Prefabs"))
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

#include "AssetManager.h"

namespace Chilli {

	void AssetManager::LoadAssets(const rapidjson::Value& assets)
	{
        m_meshes.clear();
        m_textures.clear();
        m_materials.clear();
        AddDefaultMaterial();
        for (unsigned int i = 0; i < assets.Size(); i++)
        {
            switch (assets[i]["Type"].GetInt())
            {
                case (int)AssetType::Mesh:
                    m_meshes.insert({ assets[i]["Uuid"].GetUint64(),std::make_shared<Mesh>(m_projDir +"/" + assets[i]["FilePath"].GetString(),
                        assets[i]["Uuid"].GetUint64()) });
                    break;
                case (int)AssetType::Texture:
                    m_textures.insert({ assets[i]["Uuid"].GetUint64(),std::make_shared<Texture>(m_projDir + "/" + assets[i]["FilePath"].GetString(),
                        assets[i]["Uuid"].GetUint64()) });
                    break;
                case (int)AssetType::Material:
                {
                    Material mat;
                    mat.Name = assets[i]["Name"].GetString();
                    mat.Id = assets[i]["Uuid"].GetUint64();
                    mat.DiffuseColor[0] = assets[i]["DiffR"].GetFloat();
                    mat.DiffuseColor[1] = assets[i]["DiffG"].GetFloat();
                    mat.DiffuseColor[2] = assets[i]["DiffB"].GetFloat();
                    mat.SpecularColor[0] = assets[i]["SpecR"].GetFloat();
                    mat.SpecularColor[1] = assets[i]["SpecG"].GetFloat();
                    mat.SpecularColor[2] = assets[i]["SpecB"].GetFloat();
                    mat.Shininess = assets[i]["Shininess"].GetFloat();
                    mat.DiffuseTexId = assets[i]["DiffTexUuid"].GetUint64();
                    mat.SpecularTexId = assets[i]["SpecTexUuid"].GetUint64();
                    m_materials.emplace( assets[i]["Uuid"].GetUint64(), mat);
                }
                break;
                default:
                    break;
            }
        }
	}

    void AssetManager::Reset()
    {
        m_meshes.clear();
        m_textures.clear();
    }

    std::string AssetManager::SaveAssets()const
    {
        std::stringstream ss;
        for (const auto& mesh: m_meshes)
        {
            ss << mesh.second->Serialize() << ",";
        }
        for (const auto& texture : m_textures)
        {
            ss << texture.second->Serialize() << ",";
        }
        for (const auto& material : m_materials)
        {
            ss << material.second.Serialize() << ",";
        }
        ss.seekp(-1, std::ios_base::end);
        ss << " ";
        return ss.str();
    }

    void AssetManager::AddMesh(const std::filesystem::path& filename)
    {
        auto mesh = std::make_shared<Mesh>(filename);
        m_meshes.emplace( mesh->Uuid.Get(), std::move(mesh));
    }

    void AssetManager::RemoveMesh(UUID uuid)
    {
        auto mesh = m_meshes.find(uuid.Get());
        if (mesh != m_meshes.end())
            m_meshes.erase(mesh);
    }

    void AssetManager::RemoveTexture(UUID uuid)
    {
        auto texture = m_textures.find(uuid.Get());
        if (texture != m_textures.end())
            m_textures.erase(texture);
    }

    void AssetManager::AddTexture(const std::filesystem::path& filename)
    {
        auto texture = std::make_shared<Texture>(filename);
        m_textures.emplace(texture->Uuid.Get(), std::move(texture));
    }

    void AssetManager::AddScript(const std::filesystem::path& filename)
    {
        std::string basePath(m_projDir + "Assets\\Scripts\\");
        std::string fullPath = basePath + filename.string()+".cs";
        std::ofstream script(fullPath);
        script << "using Chilli;"
        << "namespace Application {" 
        << "public class" << filename.string() << ": ChilliScript"
        << "{"
        << "public override void OnCreate()" 
        <<"{}"
        << "public override void OnDestroy() {}"
        << "public override void OnUpdate(float dt) {}"
        <<  "}" 
        <<"}";
        script.close();
    }

    std::shared_ptr<Mesh> AssetManager::GetMeshByUUID(UUID uuid)const
    {
        auto mesh = m_meshes.find(uuid.Get());
        if (mesh != m_meshes.end())
            return mesh->second;

        return nullptr;
    }

    std::shared_ptr<Texture> AssetManager::GetTextureByUUID(UUID uuid)const
    {
        auto texture = m_textures.find(uuid.Get());
        if (texture != m_textures.end())
            return texture->second;

        return nullptr;
    }

    const std::unordered_map<uint64_t, std::shared_ptr<Mesh>>& AssetManager::GetMeshes()const
    {
        return m_meshes;
    }

    const std::unordered_map<uint64_t, std::shared_ptr<Texture>>& AssetManager::GetTextures()const
    {
        return m_textures;
    }
    const std::unordered_map<uint64_t, Material>& AssetManager::GetMaterials() const
    {
        return m_materials;
    }
    const Material& AssetManager::GetMaterial(uint64_t materialId)
    {
        auto matItr = m_materials.find(materialId);
        if (matItr != m_materials.end())
            return matItr->second;
        return m_materials[0];
    }
    void AssetManager::AddMaterial(Material mat)
    {
        m_materials.emplace(mat.Id.Get(), mat);
    }
    void AssetManager::RemoveMaterial(const UUID matId)
    {
        auto matItr = m_materials.find(matId.Get());
        if (matItr != m_materials.end())
            m_materials.erase(matItr);
    }
    void AssetManager::AddDefaultMaterial()
    {
        Material mat = {};
        mat.Name = "Default";
        m_materials.emplace(0, mat);
    }
}
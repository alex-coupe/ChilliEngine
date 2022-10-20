#include "AssetManager.h"

namespace Chilli {

	void AssetManager::LoadAssets(const rapidjson::Value& assets)
	{
        m_meshes.clear();
        m_textures.clear();
        for (unsigned int i = 0; i < assets.Size(); i++)
        {
            switch (assets[i]["Type"].GetInt())
            {
                case (int)AssetType::Mesh:
                    m_meshes.insert({ assets[i]["Uuid"].GetUint64(),std::make_shared<Mesh>(assets[i]["FilePath"].GetString(),
                        assets[i]["Uuid"].GetUint64()) });
                    break;
                case (int)AssetType::Texture:
                    m_textures.insert({ assets[i]["Uuid"].GetUint64(),std::make_shared<Texture>(assets[i]["FilePath"].GetString(),
                        assets[i]["Uuid"].GetUint64()) });
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
        std::string basePath("Assets\\Scripts\\");
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
}
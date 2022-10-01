#include "AssetManager.h"

namespace Chilli {

	void AssetManager::LoadAssets(const rapidjson::Value& assets)
	{
        m_meshes.clear();
        for (unsigned int i = 0; i < assets.Size(); i++)
        {
            switch (assets[i]["Type"].GetInt())
            {
                case (int)AssetType::Mesh:
                    m_meshes.insert({ assets[i]["Uuid"].GetUint64(),std::make_shared<Mesh>(assets[i]["FilePath"].GetString(),
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
    }

    std::string AssetManager::SaveAssets()const
    {
        std::stringstream ss;
        for (const auto& mesh: m_meshes)
        {
            ss << mesh.second->Serialize() << ",";
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

    std::shared_ptr<Mesh> AssetManager::GetMeshByUUID(UUID uuid)const
    {
        auto mesh = m_meshes.find(uuid.Get());
        if (mesh != m_meshes.end())
            return mesh->second;

        return nullptr;
    }

    const std::unordered_map<uint64_t, std::shared_ptr<Mesh>>& AssetManager::GetMeshes()const
    {
        return m_meshes;
    }
}
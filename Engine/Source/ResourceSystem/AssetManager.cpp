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

    void AssetManager::AddScript(const std::string& classname)const
    {
        std::stringstream ss;
        ss << "using Chilli; namespace Application { class " << classname << ": ChilliScript {"
            << "public override void OnCreate() {} public override void OnDestroy() {} public override void OnUpdate(float dt) {}"
            << "}}";

        std::ofstream outputStream;
        std::stringstream filename;
        filename << "Assets/Scripts/" << classname << ".cs";
        outputStream.open(filename.str());
        outputStream << ss.str();
        outputStream.close();
    }

    void AssetManager::AddScript(const std::shared_ptr<Script>& script)
    {
        m_scripts.emplace(script->Uuid.Get(), script);
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

    std::shared_ptr<Script> AssetManager::GetScriptByUUID(UUID uuid)const
    {
        auto script = m_scripts.find(uuid.Get());
        if (script != m_scripts.end())
            return script->second;

        return nullptr;
    }

    std::shared_ptr<Script> AssetManager::GetScriptByName(const std::string& name)const
    {
        for (const auto script : m_scripts)
        {
            if (script.second->GetScriptName() == name)
                return script.second;
        }

        return nullptr;
    }

    const std::unordered_map<uint64_t, std::shared_ptr<Mesh>>& AssetManager::GetMeshes()const
    {
        return m_meshes;
    }

    const std::unordered_map<uint64_t, std::shared_ptr<Script>>& AssetManager::GetScripts()const
    {
        return m_scripts;
    }
}
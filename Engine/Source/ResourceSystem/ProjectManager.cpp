#include "ProjectManager.h"
#include "../Rendering/Renderer.h"
#include "../Core/DependencyResolver.h"

namespace Chilli {

    ProjectManager::ProjectManager()
    {
        m_sceneManager = std::make_unique<SceneManager>();
        m_assetManager = std::make_unique<AssetManager>();
        for (const auto& script : m_sceneManager->BuildAvailableScripts())
        {
            m_assetManager->AddScript(script);
        }
    }

    void ProjectManager::LoadProject(const std::string& filename)
    {
        std::stringstream ss;
        std::ifstream json;
        json.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        json.open(filename);
        try
        {
            ss << json.rdbuf();
        }
        catch (std::ifstream::failure&)
        {
            CHILLI_ERROR("Loading JSON File Failed");
        }

        rapidjson::Document document;

        auto payload = ss.str();

        if (document.Parse(payload.c_str()).HasParseError())
            return;

        if (document.HasMember("ProjectName"))
        {
            m_projectName = document["ProjectName"].GetString();
        }

        if (document.HasMember("Assets"))
        {
            m_assetManager->LoadAssets(document["Assets"].GetArray());
        }

        if (document.HasMember("Scenes"))
        {
            m_sceneManager->LoadScenes(document["Scenes"].GetArray());
        }
    }

    void ProjectManager::SaveProject(const std::string& filename)
    {
        std::stringstream ss;
        std::ofstream outputStream;

        ss << "{ \"ProjectName\":\"" << m_projectName << "\", \"Assets\":["
        << m_assetManager->SaveAssets() << "], \"Scenes\":[ " 
        << m_sceneManager->SaveScenes() << "]}";
        outputStream.open(filename);
        outputStream << ss.str();
        outputStream.close();
    }

    void ProjectManager::NewProject()
    {
        m_assetManager->Reset();
        m_sceneManager->Reset();
        m_projectName = "Untitled Project";
    }

    void ProjectManager::AddScene(const std::string& name)
    {
        m_sceneManager->AddScene(name);
    }

    std::vector<std::shared_ptr<Scene>> ProjectManager::GetScenes() const
    {
        return m_sceneManager->GetAllScenes();
    }

    void ProjectManager::AddScript(const std::string& className)
    {
        m_assetManager->AddScript(className);
    }

    void ProjectManager::AddAsset(const std::filesystem::path& filename, AssetType type)
    {
        switch (type)
        {
        case AssetType::Mesh:
            m_assetManager->AddMesh(filename);
            break;
        default:
            break;
        }
    }

    void ProjectManager::RemoveAsset(UUID uuid,AssetType type)
    {
        switch (type)
        {
        case AssetType::Mesh:
            m_assetManager->RemoveMesh(uuid);
            break;
        default:
            return;
        }   
    }

    void ProjectManager::RemoveScene(UUID uuid)
    {
        m_sceneManager->RemoveScene(uuid);
    }

    std::shared_ptr<Scene> ProjectManager::GetCurrentScene() const
    {
        return m_sceneManager->GetCurrentScene();
    }

    void ProjectManager::SetCurrentScene(UUID uuid)
    {
        m_sceneManager->GoToScene(uuid);
    }

    std::shared_ptr<Asset> ProjectManager::GetAssetByUUID(UUID uuid, AssetType type)
    {
        switch (type)
        {
        case AssetType::Mesh:
            return m_assetManager->GetMeshByUUID(uuid);
        case AssetType::Script:
            return m_assetManager->GetScriptByUUID(uuid);
        default:
            return nullptr;
        }
    }

    const std::unordered_map<uint64_t, std::shared_ptr<Script>>& ProjectManager::GetScripts()const
    {
        return m_assetManager->GetScripts();
    }

    const std::unordered_map<uint64_t, std::shared_ptr<Mesh>>& ProjectManager::GetMeshes()const
    {
        return m_assetManager->GetMeshes();
    }

    SystemType ProjectManager::GetSystemType()
    {
        return SystemType::ProjectManager;
    }

    void ProjectManager::PlayCurrentScene()const
    {
        m_sceneManager->StartCurrentScene();
    }

    void ProjectManager::StopCurrentScene()const
    {
        m_sceneManager->StopCurrentScene();
    }

    std::shared_ptr<Script> ProjectManager::GetScriptByName(const std::string& name)const
    {
       return  m_assetManager->GetScriptByName(name);
    }

    void ProjectManager::ProcessFrame()
    {
        m_sceneManager->UpdateCurrentScene();
    }

    MonoImage* ProjectManager::GetCoreScriptAssemblyImage()const
    {
        return m_sceneManager->GetScriptEngine()->GetCoreAssemblyImage();
    }
}

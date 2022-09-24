#include "ProjectManager.h"
#include "../Rendering/Renderer.h"
#include "../Core/DependencyResolver.h"

namespace Chilli {

    ProjectManager::ProjectManager()
    {
        m_scenes.emplace_back(std::make_shared<Scene>("Scene 1"));
        m_currentScene = m_scenes.front();
        m_scriptEngine = std::make_unique<ScriptEngine>();
    }

    const std::vector<std::string>& ProjectManager::GetAvailableScripts()const
    {
        return m_scriptEngine->GetAvailableClasses();
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
            m_assets.clear();
            for (const auto& asset : document["Assets"].GetArray())
            {
                switch (asset["Type"].GetInt())
                {
                case (int)AssetTypes::Mesh:
                    m_assets.emplace_back(std::make_shared<Mesh>(asset["FilePath"].GetString(),
                        asset["Uuid"].GetUint64()));
                    break;
                default:
                    break;
                }
            }
        }

        if (document.HasMember("Scenes"))
        {
            m_scenes.clear();
            m_currentScene.reset();
            for (const auto& scene : document["Scenes"].GetArray())
            {
                m_scenes.emplace_back(std::make_shared<Scene>(scene["SceneName"].GetString(), scene["Uuid"].GetUint64(), scene["Entities"].GetArray()));
            }
            m_currentScene = m_scenes.front();
        }
    }

    void ProjectManager::SaveProject(const std::string& filename)
    {
        std::stringstream ss;
        std::ofstream outputStream;

        ss << "{ \"ProjectName\":\"" << m_projectName << "\", \"Assets\":[";
        for (size_t i = 0; i < m_assets.size(); i++)
        {
            ss << m_assets[i]->Serialize();
            if (i != m_assets.size() - 1)
                ss << ",";
        }
        ss << "], \"Scenes\":[ ";
        for (size_t i = 0; i < m_scenes.size(); i++)
        {
            ss << m_scenes[i]->Serialize();
            if (i != m_scenes.size() - 1)
                ss << ",";
        }
        ss << "]}";
        outputStream.open(filename);
        outputStream << ss.str();
        outputStream.close();
    }

    void ProjectManager::NewProject()
    {
        m_scenes.clear();
        m_assets.clear();
        m_projectName = "Untitled Project";
        m_scenes.emplace_back(std::make_shared<Scene>("Scene 1"));
        m_currentScene = m_scenes.front();
    }

    void ProjectManager::AddScene(const std::string& name)
    {
        m_scenes.emplace_back(std::make_shared<Scene>(name));
    }

    std::vector<std::shared_ptr<Scene>> ProjectManager::GetScenes() const
    {
        return m_scenes;
    }

    void ProjectManager::AddAsset(const std::filesystem::path& filename, AssetTypes type)
    {
        switch (type)
        {
        case AssetTypes::Mesh:
            m_assets.emplace_back(std::make_shared<Mesh>(filename));
            break;
        case AssetTypes::Script:
            m_assets.emplace_back(std::make_shared<Script>(filename));
            break;
        default:
            break;
        }
    }

    void ProjectManager::RemoveAsset(UUID uuid)
    {
        if (auto m_assetIterator = std::find_if(m_assets.begin(), m_assets.end(), [uuid](const std::shared_ptr<Asset> rhs)
            {
                return rhs->Uuid.Get() == uuid.Get();
            }); m_assetIterator != m_assets.end() && m_assets.size() > 0)
        {
            m_assets.erase(m_assetIterator);
        }
    }


    void ProjectManager::RemoveScene(UUID uuid)
    {
        if (auto m_sceneIterator = std::find_if(m_scenes.begin(), m_scenes.end(), [uuid](const std::shared_ptr<Scene> rhs)
            {
                return rhs->Uuid.Get() == uuid.Get();
            }); m_sceneIterator != m_scenes.end() && m_scenes.size() > 0)
        {
            m_scenes.erase(m_sceneIterator);
        }
    }

    std::shared_ptr<Scene> ProjectManager::GetCurrentScene() const
    {
        return m_currentScene;
    }

    void ProjectManager::SetCurrentScene(UUID uuid)
    {
        if (auto m_sceneIterator = std::find_if(m_scenes.begin(), m_scenes.end(), [&uuid](const std::shared_ptr<Scene> rhs)
            {
                return rhs->Uuid.Get() == uuid.Get();
            }); m_sceneIterator != m_scenes.end() && m_scenes.size() > 0)
        {
            m_currentScene = *m_sceneIterator;
        }
    }

    void ProjectManager::SetCurrentSceneState(SceneState state)
    {
        switch (state) {
        case SceneState::Play:
        case SceneState::Simulate:
            m_scriptEngine->SceneStart();
            m_currentScene->onSceneStart();
            break;
        case SceneState::Edit:
        case SceneState::Pause:
            m_currentScene->onSceneStop();
            break;
        }
        m_currentScene->SetSceneState(state);
    }

    std::shared_ptr<Asset> ProjectManager::GetAssetByUUID(UUID uuid)
    {
        if (auto m_assetsIterator = std::find_if(m_assets.begin(), m_assets.end(), [uuid](const std::shared_ptr<Asset> rhs)
            {
                return rhs->Uuid.Get() == uuid.Get();
            }); m_assetsIterator != m_assets.end() && m_assets.size() > 0)
        {
            return *m_assetsIterator;
        }
            return nullptr;
    }

    std::vector<std::shared_ptr<Asset>> ProjectManager::GetAssetsByType(AssetTypes type)
    {
        std::vector<std::shared_ptr<Asset>> assets;
        std::copy_if(m_assets.begin(), m_assets.end(), std::back_inserter(assets), [type](const std::shared_ptr<Asset> rhs) {
            return rhs->GetAssetType() == type;
            });
        return assets;
    }

    std::vector<std::shared_ptr<Component>> ProjectManager::GetCurrentSceneComponentsByType(ComponentTypes type)
    {
        std::vector<std::shared_ptr<Component>> components;
        for (const auto& ent : m_currentScene->GetEntities())
        {
            if (const auto& comp = ent->GetComponentByType(type); comp != nullptr)
                components.push_back(comp);
        }
        return components;
    }
    SystemType ProjectManager::GetSystemType()
    {
        return SystemType::ProjectManager;
    }

    void ProjectManager::ProcessFrame()
    {
        if (m_currentScene->GetSceneState() == SceneState::Play || m_currentScene->GetSceneState() == SceneState::Simulate)
        {
            m_scriptEngine->SceneUpdate();
            m_currentScene->onSceneUpdate();
        }
    }
}

#include "ProjectManager.h"
#include "../Rendering/Renderer.h"
#include "../Core/DependencyResolver.h"

Engine::ResourceSystem::ProjectManager::ProjectManager()
{
    m_scenes.emplace_back(std::make_shared<Scene>("Scene 1"));
    m_currentScene = m_scenes.front();
}

void Engine::ResourceSystem::ProjectManager::LoadProject(const std::string& filename)
{
    std::ifstream json;
    json.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    json.open(filename);
    try
    {
        ss << json.rdbuf();
    }
    catch (std::ifstream::failure&)
    {
        throw std::runtime_error("Loading JSON File Failed");
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
                        asset["Uuid"].GetString()));
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
            m_scenes.emplace_back(std::make_shared<Scene>(scene["SceneName"].GetString(), scene["Uuid"].GetString(), scene["Entities"].GetArray()));
        }
        m_currentScene = m_scenes.front();
    }
}

void Engine::ResourceSystem::ProjectManager::SaveProject(const std::string& filename)
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

void Engine::ResourceSystem::ProjectManager::NewProject()
{
    m_scenes.clear();
    m_scenes.emplace_back(std::make_shared<Scene>("Scene 1"));
    m_currentScene = m_scenes.front();
}

void Engine::ResourceSystem::ProjectManager::AddScene(const std::string& name)
{
    m_scenes.emplace_back(std::make_shared<Scene>(name));
}

std::vector<std::shared_ptr<Engine::ResourceSystem::Scene>> Engine::ResourceSystem::ProjectManager::GetScenes() const
{
    return m_scenes;
}

void Engine::ResourceSystem::ProjectManager::AddAsset(const std::filesystem::path& filename, AssetTypes type)
{
    switch (type)
    {
        case AssetTypes::Mesh:
            m_assets.emplace_back(std::make_shared<Mesh>(filename, Engine::Utilities::UUID()));
            break;
        default:
            break;
    }
}

void Engine::ResourceSystem::ProjectManager::RemoveAsset(const Engine::Utilities::UUID& uuid)
{
    if (auto m_assetIterator = std::find_if(m_assets.begin(), m_assets.end(), [uuid](const std::shared_ptr<Asset> rhs)
        {
            return rhs->GetUUID() == uuid;
        }); m_assetIterator != m_assets.end() && m_assets.size() > 0)
    {
        m_assets.erase(m_assetIterator);
    }
}


void Engine::ResourceSystem::ProjectManager::RemoveScene(Engine::Utilities::UUID& uuid)
{
    if (auto m_sceneIterator = std::find_if(m_scenes.begin(), m_scenes.end(), [uuid](const std::shared_ptr<Scene> rhs)
        {
            return rhs->GetUUID() == uuid;
        }); m_sceneIterator != m_scenes.end() && m_scenes.size() > 0)
    {
        m_scenes.erase(m_sceneIterator);
    }
}

std::shared_ptr<Engine::ResourceSystem::Scene> Engine::ResourceSystem::ProjectManager::GetCurrentScene() const
{
    return m_currentScene;
}

void Engine::ResourceSystem::ProjectManager::SetCurrentScene(Engine::Utilities::UUID& uuid)
{
    if (auto m_sceneIterator = std::find_if(m_scenes.begin(), m_scenes.end(), [&uuid](const std::shared_ptr<Scene> rhs)
        {
            return rhs->GetUUID() == uuid;
        }); m_sceneIterator != m_scenes.end() && m_scenes.size() > 0)
    {
        m_currentScene = *m_sceneIterator;
    }
}

std::shared_ptr<Engine::ResourceSystem::Asset> Engine::ResourceSystem::ProjectManager::GetAssetByUUID(Engine::Utilities::UUID& uuid)
{
    if (auto m_assetsIterator = std::find_if(m_assets.begin(), m_assets.end(), [&uuid](const std::shared_ptr<Engine::ResourceSystem::Asset> rhs)
        {
            return rhs->GetUUID() == uuid;
        }); m_assetsIterator != m_assets.end() && m_assets.size() > 0)
    {
        return *m_assetsIterator;
    }
    return nullptr;
}

std::vector<std::shared_ptr<Engine::ResourceSystem::Asset>> Engine::ResourceSystem::ProjectManager::GetAssetsByType(AssetTypes type)
{
    std::vector<std::shared_ptr<Asset>> assets;
    std::copy_if(m_assets.begin(), m_assets.end(), std::back_inserter(assets), [&type](const std::shared_ptr<Asset> rhs) {
        return rhs->GetAssetType() == type;
        });
    return assets;
}

int Engine::ResourceSystem::ProjectManager::GetSystemType() const
{
    return static_cast<int>(Engine::Core::SystemTypes::ProjectManager);
}

void Engine::ResourceSystem::ProjectManager::ProcessFrame()
{
    m_currentScene->Update(Engine::Core::DependencyResolver::ResolveDependency<Engine::Core::Timer>()->GetDeltaTime());
}

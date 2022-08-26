#include "SceneManager.h"
#include <Windows.h>
#include "../Rendering/Renderer.h"
#include "../Core/DependencyResolver.h"

Engine::ResourceSystem::SceneManager::SceneManager()
{
    m_scenes.emplace_back(std::make_shared<Scene>("Scene 1"));
    m_currentScene = m_scenes.front();
    LoadProject("sample.json");
}

void Engine::ResourceSystem::SceneManager::LoadProject(const std::string& filename)
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

void Engine::ResourceSystem::SceneManager::SaveProject(const std::string& filename)
{
    std::stringstream ss;
    std::ofstream outputStream;

    ss << "{ \"ProjectName\":\"" << m_projectName << "\", \"Scenes\":[ ";
    for (const auto& scene : m_scenes)
    {
        ss << scene->Serialize();
    }
    ss << "]}";
    outputStream.open(filename);
    outputStream << ss.str();
    outputStream.close();
}

void Engine::ResourceSystem::SceneManager::AddScene(const std::string& name)
{
    m_scenes.emplace_back(std::make_shared<Scene>(name));
}

void Engine::ResourceSystem::SceneManager::RemoveScene(Engine::Utilities::UUID& uuid)
{
    if (auto m_sceneIterator = std::find_if(m_scenes.begin(), m_scenes.end(), [uuid](const std::shared_ptr<Scene> rhs)
        {
            return rhs->GetUUID() == uuid;
        }); m_sceneIterator != m_scenes.end() && m_scenes.size() > 1)
    {
        m_scenes.erase(m_sceneIterator);
    }
}

std::shared_ptr<Engine::ResourceSystem::Scene> Engine::ResourceSystem::SceneManager::GetCurrentScene() const
{
    return m_currentScene;
}

void Engine::ResourceSystem::SceneManager::SetCurrentScene(Engine::Utilities::UUID& uuid)
{
    if (auto m_sceneIterator = std::find_if(m_scenes.begin(), m_scenes.end(), [uuid](const std::shared_ptr<Scene> rhs)
        {
            return rhs->GetUUID() == uuid;
        }); m_sceneIterator != m_scenes.end() && m_scenes.size() > 1)
    {
        m_currentScene = *m_sceneIterator;
    }
}

int Engine::ResourceSystem::SceneManager::GetSystemType() const
{
    return static_cast<int>(Engine::Core::SystemTypes::SceneManager);
}

void Engine::ResourceSystem::SceneManager::ProcessFrame()
{
    m_currentScene->Update(Engine::Core::DependencyResolver::ResolveDependency<Engine::Core::Timer>()->GetDeltaTime());
}

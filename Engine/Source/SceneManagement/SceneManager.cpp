#include "SceneManager.h"
#include <Windows.h>
#include "../Rendering/Renderer.h"

Engine::SceneManagement::SceneManager::SceneManager(const std::shared_ptr<Engine::Core::DependencyResolver<SubSystem>>& resolver)
    : SubSystem(resolver)
{
    m_scenes.emplace_back(std::make_shared<Scene>(1, "Scene 1"));
    m_currentScene = m_scenes.front();
}

void Engine::SceneManagement::SceneManager::DrawGui()
{
    ImGui::Begin("Debug");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
        1000.0 / float(ImGui::GetIO().Framerate), float(ImGui::GetIO().Framerate));
    if (ImGui::Button("Save Project"))
    {
        SaveProject("sample.json");
    }
    if (ImGui::Button("Load Project"))
    {
        LoadProject("sample.json");
    }
    for (const auto& scene : m_scenes)
    {
        ImGui::Button(scene->GetName().c_str());
    }

    ImGui::End();
    m_currentScene->DrawGui();
                        
}

void Engine::SceneManagement::SceneManager::LoadProject(const std::string& filename)
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
            m_scenes.emplace_back(std::make_shared<Scene>(scene["Id"].GetInt(), scene["SceneName"].GetString(), scene["Entities"].GetArray()));
        }
        m_currentScene = m_scenes.front();
    }
}

void Engine::SceneManagement::SceneManager::SaveProject(const std::string& filename)
{
    std::stringstream ss;
    std::ofstream outputStream;
    for (const auto& scene : m_scenes)
    {
        ss << scene->Serialize();
    }

    outputStream.open(filename);
    outputStream << ss.str();
    outputStream.close();
}

void Engine::SceneManagement::SceneManager::AddScene(const std::string& name)
{
    m_scenes.emplace_back(std::make_shared<Scene>((unsigned int)m_scenes.size(), name));
}

void Engine::SceneManagement::SceneManager::RemoveScene(unsigned int scene_id)
{
    
}

unsigned int Engine::SceneManagement::SceneManager::GetCurrentSceneId() const
{
    return m_currentSceneId;
}

std::shared_ptr<Engine::SceneManagement::Scene> Engine::SceneManagement::SceneManager::GetCurrentScene() const
{
    return m_currentScene;
}

void Engine::SceneManagement::SceneManager::SetCurrentScene(unsigned int scene_id)
{
    //Maybe check the scene exists first?
    m_currentSceneId = scene_id;
}

void Engine::SceneManagement::SceneManager::DrawCurrentScene() const
{
    m_currentScene->Draw();
}

int Engine::SceneManagement::SceneManager::GetHash() const
{
    return static_cast<int>(Engine::Core::SystemTypes::SceneManager);
}

void Engine::SceneManagement::SceneManager::ProcessFrame()
{
   // m_currentScene->Update(0.1f);
}



#include "SceneManager.h"
#include "../Gui/GuiManager.h"

Engine::SceneManagement::SceneManager::SceneManager(const std::shared_ptr<Engine::Core::DependencyResolver<SubSystem>>& resolver)
    : SubSystem(resolver)
{
    m_scenes.emplace_back(std::make_shared<Scene>(1, "Scene 1"));
}

void Engine::SceneManagement::SceneManager::DrawGui() const
{
    bool drawMenu = true;
    ImGui::Begin("Scene Manager",&drawMenu,  ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
            if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Do stuff */ }
            if (ImGui::MenuItem("Close", "Ctrl+W")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    static const char* current_item = NULL;
    if (ImGui::BeginCombo("Scenes", current_item, ImGuiComboFlags_NoArrowButton & ImGuiComboFlags_NoPreview))
    {
        for (const auto& scene : m_scenes)
            if (ImGui::Selectable(scene->GetName().c_str(), false))
                current_item = scene->GetName().c_str();
        ImGui::EndCombo();
    }
    ImGui::End();
   
   
}

void Engine::SceneManagement::SceneManager::AddScene(const std::string& name)
{
    m_scenes.emplace_back(std::make_shared<Scene>(1, name));
}

void Engine::SceneManagement::SceneManager::RemoveScene(unsigned int scene_id)
{
    /*
    std::remove_if(m_scenes.begin(), m_scenes.end(), [this, scene_id](const Scene& rhs) {
        return rhs.GetID() == scene_id && scene_id != m_currentSceneId;
        });*/
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

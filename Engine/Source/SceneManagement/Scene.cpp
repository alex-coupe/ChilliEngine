#include "Scene.h"

Engine::SceneManagement::Scene::Scene(unsigned int id, const std::string& name)
    :m_id(id), m_name(name)
{
}

Engine::SceneManagement::Scene::Scene(unsigned int id, const std::string& name, unsigned int last_scene_id, const rapidjson::Value& entities)
    : m_id(id), m_name(name)
{
    for (unsigned int i = 0; i < entities.Size(); i++)
    {
        m_entities.emplace_back(std::make_shared<Entity>(entities[i]["Name"].GetString(), entities[i]["Id"].GetInt(), entities[i]["Components"].GetArray()));
    }
}

const std::string Engine::SceneManagement::Scene::Serialize()
{
    std::stringstream ss;
    ss << "{ \"SceneName\":\"" << m_name << "\", \"Id\":" << m_id << ", \"LastEntityId\":" << m_lastEntityId << ", \"Entities\": [";
    for (const auto& entity : m_entities)
    {
        ss << entity->Serialize();
    }
    ss << "]}";
    return ss.str();
}

void Engine::SceneManagement::Scene::AddEntity(const std::string& name)
{
    m_entities.emplace_back(std::make_shared<Entity>(name, m_lastEntityId++));
}

void Engine::SceneManagement::Scene::RemoveEntity(unsigned int id)
{
    if (auto m_entIterator = std::find_if(m_entities.begin(), m_entities.end(), [id](const std::shared_ptr<Entity> rhs)
        {
            return rhs->GetId() == id;
        }); m_entIterator != m_entities.end())
    {
        m_entities.erase(m_entIterator);
    }
}

const std::vector<std::shared_ptr<Engine::SceneManagement::Entity>>& Engine::SceneManagement::Scene::GetEntities() const
{
    return m_entities;
}

void Engine::SceneManagement::Scene::Update(float dt)
{
}

void Engine::SceneManagement::Scene::DrawGui()
{
    for (int i = 0; i < m_entities.size(); i++)
    {
        m_entities[i]->DrawGui();
        if (ImGui::Button("Remove Entity"))
        {
            RemoveEntity(m_entities[i]->GetId());
        }
        ImGui::End();
    }
}

unsigned int Engine::SceneManagement::Scene::GetId() const
{
    return m_id;
}

const std::string& Engine::SceneManagement::Scene::GetName() const
{
    return m_name;
}

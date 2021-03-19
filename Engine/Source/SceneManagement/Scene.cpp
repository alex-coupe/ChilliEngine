#include "Scene.h"

Engine::SceneManagement::Scene::Scene(unsigned int id, const std::string& name)
    :m_id(id), m_name(name)
{
}

Engine::SceneManagement::Scene::Scene(unsigned int id, const std::string& name, const rapidjson::Value& entities)
    : m_id(id), m_name(name)
{
    for (unsigned int i = 0; i < entities.Size(); i++)
    {
        m_entities.emplace_back(std::make_shared<Entity>(entities[i]["Name"].GetString(), entities[i]["Id"].GetInt(), entities[i]["Components"].GetArray()));
    }
}

void Engine::SceneManagement::Scene::Serialize(const std::string& filename)
{
}

void Engine::SceneManagement::Scene::Deserialize(const std::string& filename)
{
}

void Engine::SceneManagement::Scene::AddEntity()
{
}

void Engine::SceneManagement::Scene::RemoveEntity()
{
}

const std::vector<std::shared_ptr<Engine::SceneManagement::Entity>>& Engine::SceneManagement::Scene::GetEntities() const
{
    return m_entities;
}

void Engine::SceneManagement::Scene::Update(float dt)
{
}

void Engine::SceneManagement::Scene::DrawGui() const
{
    for (const auto& ent : m_entities)
    {
        ent->DrawGui();
    }
}

void Engine::SceneManagement::Scene::Draw() const
{
}

unsigned int Engine::SceneManagement::Scene::GetID() const
{
    return m_id;
}

const std::string& Engine::SceneManagement::Scene::GetName() const
{
    return m_name;
}

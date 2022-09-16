#include "Scene.h"

Engine::ResourceSystem::Scene::Scene( const std::string& name)
    : m_name(name), m_uuid()
{  
}

Engine::ResourceSystem::Scene::Scene(const std::string& name, Engine::Utilities::UUID uuid, const rapidjson::Value& entities)
    : m_name(name), m_uuid(uuid)
{
    for (unsigned int i = 0; i < entities.Size(); i++)
    {
        m_entities.emplace_back(std::make_shared<Entity>(entities[i]["Name"].GetString(), entities[i]["Uuid"].GetString(), entities[i]["Components"].GetArray()));
    }
}

Engine::ResourceSystem::Scene::Scene(const Scene& rhs)
{
    for (const auto entity : rhs.GetEntities())
    {
        m_entities.emplace_back(std::make_shared<Entity>(*entity));
    }
}

const std::string Engine::ResourceSystem::Scene::Serialize()
{
    std::stringstream ss;
    ss << "{ \"SceneName\":\"" << m_name << "\", \"Uuid\": \"" << m_uuid.GetUUID() << "\", \"Entities\": [";
    for (size_t i = 0; i < m_entities.size(); i++)
    {
        ss << m_entities[i]->Serialize();
        if (i != m_entities.size() - 1)
            ss << ",";
    }
    ss << "]}";
    return ss.str();
}

void Engine::ResourceSystem::Scene::AddEntity(const std::string& name)
{
    m_entities.emplace_back(std::make_shared<Entity>(name));
}

void Engine::ResourceSystem::Scene::RemoveEntity(const Engine::Utilities::UUID& uuid)
{
    if (auto m_entIterator = std::find_if(m_entities.begin(), m_entities.end(), [uuid](const std::shared_ptr<Entity> rhs)
        {
            return rhs->GetUUID() == uuid;
        }); m_entIterator != m_entities.end())
    {
        m_entities.erase(m_entIterator);
    }
}

const std::vector<std::shared_ptr<Engine::ResourceSystem::Entity>>& Engine::ResourceSystem::Scene::GetEntities() const
{
    return m_entities;
}

const Engine::ResourceSystem::SceneState Engine::ResourceSystem::Scene::GetSceneState()const
{
    return m_sceneState;
}

void Engine::ResourceSystem::Scene::SetSceneState(SceneState state)
{
    m_sceneState = state;
}

void Engine::ResourceSystem::Scene::onSceneStart()
{
    m_b2World = std::make_unique<b2World>(m_gravity);
    for (const auto& entities : m_entities)
    {
        entities->OnSceneStart(m_b2World);
    }
}

void Engine::ResourceSystem::Scene::onSceneUpdate()
{
    m_b2World->Step(m_physicsTimestep, m_velocityIterations, m_positionIterations);
    for (const auto& entities : m_entities)
    {
        entities->OnSceneUpdate();
    }
}

void Engine::ResourceSystem::Scene::onSceneEnd()
{
}

const Engine::Utilities::UUID& Engine::ResourceSystem::Scene::GetUUID() const
{
    return m_uuid;
}

const std::string& Engine::ResourceSystem::Scene::GetName() const
{
    return m_name;
}

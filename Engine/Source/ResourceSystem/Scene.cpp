#include "Scene.h"

namespace Chilli {

    Scene::Scene(const std::string& name)
        : m_name(name), Uuid()
    {
    }

    Scene::Scene(const std::string& name, UUID uuid, const rapidjson::Value& entities)
        : m_name(name), Uuid(uuid)
    {
        for (unsigned int i = 0; i < entities.Size(); i++)
        {
            m_entities.emplace_back(std::make_shared<Entity>(entities[i]["Name"].GetString(), entities[i]["Uuid"].GetUint64(), entities[i]["Components"].GetArray()));
        }
    }

    const std::string Scene::Serialize()
    {
        std::stringstream ss;
        ss << "{ \"SceneName\":\"" << m_name << "\", \"Uuid\":" << Uuid.Get() << ", \"Entities\": [";
        for (size_t i = 0; i < m_entities.size(); i++)
        {
            ss << m_entities[i]->Serialize();
            if (i != m_entities.size() - 1)
                ss << ",";
        }
        ss << "]}";
        return ss.str();
    }

    void Scene::AddEntity(const std::string& name)
    {
        m_entities.emplace_back(std::make_shared<Entity>(name));
    }

    void Scene::RemoveEntity(UUID uuid)
    {
        if (auto m_entIterator = std::find_if(m_entities.begin(), m_entities.end(), [uuid](const std::shared_ptr<Entity> rhs)
            {
                return rhs->Uuid.Get() == uuid.Get();
            }); m_entIterator != m_entities.end())
        {
            for (int i = 0; i < m_entIterator->get()->GetComponents().size(); i++)
            {
                m_entIterator->get()->RemoveComponent(m_entIterator->get()->GetComponents()[i]->GetComponentType());
            }
            m_entities.erase(m_entIterator);
        }
    }

    const std::vector<std::shared_ptr<Entity>>& Scene::GetEntities() const
    {
        return m_entities;
    }

    const SceneState Scene::GetSceneState()const
    {
        return m_sceneState;
    }

    void Scene::SetSceneState(SceneState state)
    {
        m_sceneState = state;
    }

    void Scene::StartScene()
    {
        m_b2World = std::make_unique<b2World>(m_gravity);
        for (const auto& entity : m_entities)
        {
            auto clone = Entity::Clone(*entity);
            m_entitiesClone.emplace_back(clone);
            entity->InitPhysics(m_b2World);
        }
    }

    void Scene::UpdateScene()
    {
        m_b2World->Step(m_physicsTimestep, m_velocityIterations, m_positionIterations);
        for (const auto& entities : m_entities)
        {
            entities->UpdatePhysics();
        }
    }

    void Scene::StopScene()
    {
        m_entities.clear();
        m_entities = m_entitiesClone;
        m_entitiesClone.clear();
    }

    const std::string& Scene::GetName() const
    {
        return m_name;
    }
}
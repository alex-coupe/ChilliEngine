#include "Scene.h"
#include "../Rendering/Renderer.h"

namespace Chilli {

    Scene::Scene(const std::string& name)
        : m_name(name), Uuid()
    {}

    Scene::Scene(const std::string& name, UUID uuid, const rapidjson::Value& entities)
        : m_name(name), Uuid(uuid)
    {
        for (unsigned int i = 0; i < entities.Size(); i++)
        {
            m_entities.emplace_back(std::make_shared<Entity>(entities[i]["Uuid"].GetUint64(), entities[i]["Parent"].GetUint64(), entities[i]["Children"].GetArray(), entities[i]["Components"].GetArray()));
        }
    }

    Scene::Scene(const Scene& rhs)
        :Uuid(rhs.Uuid), m_name(rhs.m_name)
    {
        for (const auto& entity : rhs.m_entities)
        {
            m_entities.emplace_back(std::make_shared<Entity>(*entity));
        }
    }

    void Scene::Clone(const std::shared_ptr<Scene>& scene)
    {
        m_name = scene->m_name;
        for (int i = 0; i < scene->m_entities.size(); i++)
        {
            m_entities[i]->Clone(*scene->m_entities[i]);
        }
    }

    void Scene::DuplicateEntity(const std::shared_ptr<Entity> entity)
    {
        auto& dupe = m_entities.emplace_back(std::make_shared<Entity>(entity->GetName() + " - Duplicate", entity->GetParentId()));
        if (dupe->HasParent())
        {
            dupe->GetParent()->AddChild(dupe->Uuid);
        }

        dupe->RemoveComponent(ComponentType::Transform);
        for (auto& comp : entity->GetComponents())
        {
            if (comp->GetComponentType() != ComponentType::ID)
            {
                dupe->AddComponent(comp->GetComponentType());
                auto newComp = dupe->GetComponentByType(comp->GetComponentType());
                newComp->Clone(comp);

                if (comp->GetComponentType() == ComponentType::Script)
                    ScriptInstanceRepository::MakeScriptInstance(std::static_pointer_cast<ScriptComponent>(comp)->GetScriptName(), dupe->Uuid.Get());
            }
        }
        std::static_pointer_cast<IDComponent>(dupe->GetComponentByType(ComponentType::ID))->SetName(entity->GetName() + " - Copy");
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
        m_entities.emplace_back(std::make_shared<Entity>(name, 0));
    }

    void Scene::AddEntity(const std::shared_ptr<Entity> ent)
    {
        m_entities.emplace_back(ent);
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
            auto ent = m_entIterator->get();
            if (ent->HasParent())
                ent->GetParent()->RemoveChild(ent->Uuid);

            if (ent->HasChildren())
                for (const auto& child : ent->GetChildren())
                    child->SetParent(0);

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
        ScriptEngine::InvokeCreateMethod();
        m_b2World = std::make_unique<b2World>(m_gravity);
        for (const auto& entity : m_entities)
        {
            if (entity->HasComponent(ComponentType::Camera))
            {
                const auto& camComponent = std::static_pointer_cast<CameraComponent>(entity->GetComponentByType(ComponentType::Camera));
                const auto& transformComponent = std::static_pointer_cast<TransformComponent>(entity->GetComponentByType(ComponentType::Transform));
                auto renderer = DependencyResolver::ResolveDependency<Renderer>();
                m_sceneCamera = std::make_unique<Camera>(camComponent->GetFov(),
                    renderer->GetDisplayWindowAspectRatio(),
                    camComponent->GetNearClip(), camComponent->GetFarClip(),
                    CameraType::Scene, camComponent->GetProjectionType(), transformComponent->Translation());
                renderer->SetRenderCamera(m_sceneCamera.get());
            }
            if (entity->HasComponent(ComponentType::Script))
            {
                const auto& scriptInst = ScriptInstanceRepository::GetScriptInstanceByEntityId(entity->Uuid.Get());
                scriptInst->OnSceneStart();
            }
            entity->InitPhysics(m_b2World);
        }

        if (m_sceneCamera == nullptr)
            m_sceneState = SceneState::Edit;
    }

    void Scene::UpdateScene()
    {
        ScriptEngine::InvokeUpdateMethod();
        m_b2World->Step(m_physicsTimestep, m_velocityIterations, m_positionIterations);
        for (const auto& entity : m_entities)
        {
            if (entity->HasComponent(ComponentType::Camera))
            {
                const auto& camComponent = std::static_pointer_cast<CameraComponent>(entity->GetComponentByType(ComponentType::Camera));
                const auto& transformComponent = std::static_pointer_cast<TransformComponent>(entity->GetComponentByType(ComponentType::Transform));

                m_sceneCamera->UpdatePosition(transformComponent->Translation(), transformComponent->Rotation());
            }
            entity->UpdatePhysics();
        }
    }

    void Scene::StopScene()
    {
        const auto& renderer = DependencyResolver::ResolveDependency<Renderer>();
        ScriptEngine::InvokeDestroyMethod();
        for (const auto& entity : m_entities)
        {
            if (entity->HasComponent(ComponentType::Script))
            {
                const auto& scriptInst = ScriptInstanceRepository::GetScriptInstanceByEntityId(entity->Uuid.Get());
                scriptInst->OnSceneStop();
            }
        }
    }

    const std::string& Scene::GetName() const
    {
        return m_name;
    }

    void Scene::SetName(const std::string& name)
    {
        m_name = name;
    }

    std::shared_ptr<Entity> Scene::GetEntityByUUID(UUID uuid)const
    {
        if (auto m_entIterator = std::find_if(m_entities.begin(), m_entities.end(), [uuid](const std::shared_ptr<Entity> rhs)
            {
                return rhs->Uuid.Get() == uuid.Get();
            }); m_entIterator != m_entities.end())
        {
            return *m_entIterator;
        }
            return nullptr;
    }

    std::shared_ptr<Entity> Scene::GetEntityByName(const std::string& name)const
    {
        if (auto m_entIterator = std::find_if(m_entities.begin(), m_entities.end(), [name](const std::shared_ptr<Entity> rhs)
            {
                return rhs->GetName() == name;
            }); m_entIterator != m_entities.end())
        {
            return *m_entIterator;
        }
            return nullptr;
    }
}
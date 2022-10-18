#include "Scene.h"
#include "../Rendering/Renderer.h"

namespace Chilli {

    Scene::Scene(const std::string& name)
        : m_name(name), Uuid()
    {
        const auto& cameraEntity = std::make_shared<Entity>("Camera");
        cameraEntity->AddCameraComponent();
        m_entities.emplace_back(cameraEntity);
    }

    Scene::Scene(const std::string& name, UUID uuid, const rapidjson::Value& entities)
        : m_name(name), Uuid(uuid)
    {
        for (unsigned int i = 0; i < entities.Size(); i++)
        {
            m_entities.emplace_back(std::make_shared<Entity>(entities[i]["Name"].GetString(), entities[i]["Uuid"].GetUint64(), entities[i]["Components"].GetArray()));
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
        for (int i = 0; i < m_entities.size(); i++)
        {
            m_entities[i]->Clone(*scene->m_entities[i]);
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
        ScriptEngine::InvokeCreateMethod();
        m_b2World = std::make_unique<b2World>(m_gravity);
        for (const auto& entity : m_entities)
        {
            if (entity->HasComponent(ComponentType::Camera))
            {
                const auto& camComponent = std::static_pointer_cast<CameraComponent>(entity->GetComponentByType(ComponentType::Camera));
                const auto& transformComponent = std::static_pointer_cast<TransformComponent>(entity->GetComponentByType(ComponentType::Transform));
                m_sceneCamera = std::make_unique<Camera>(camComponent->GetFov(),
                    DependencyResolver::ResolveDependency<Renderer>()->GetAspectRatio(),
                    camComponent->GetNearClip(), camComponent->GetFarClip(), 
                    CameraType::Scene, camComponent->GetProjectionType(),transformComponent->Translation());
                DependencyResolver::ResolveDependency<Renderer>()->SetRenderCamera(m_sceneCamera.get());
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
        for (const auto& entities : m_entities)
        {
            entities->UpdatePhysics();
        }
    }

    void Scene::StopScene()
    {
        const auto& renderer = DependencyResolver::ResolveDependency<Renderer>();
        renderer->SetRenderCamera(renderer->GetEditorCamera().get());
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
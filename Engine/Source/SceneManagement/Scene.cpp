#include "Scene.h"

Engine::SceneManagement::Scene::Scene(unsigned int id, const std::string& name)
    :m_id(id), m_name(name)
{
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

void Engine::SceneManagement::Scene::Update(float dt)
{
}

void Engine::SceneManagement::Scene::DrawGui() const
{
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

#include "PhysicsManager.h"

Engine::Physics::PhysicsManager::PhysicsManager()
{
	m_b2World = std::make_unique<b2World>(m_gravity);
}

int Engine::Physics::PhysicsManager::GetSystemType() const
{
	return static_cast<int>(Engine::Core::SystemTypes::Physics);
}

void Engine::Physics::PhysicsManager::ProcessFrame()
{
	m_b2World->Step(m_timestep, m_velocityIterations, m_positionIterations);
	for (auto& rigidbody : Engine::Core::DependencyResolver::ResolveDependency<Engine::ResourceSystem::ProjectManager>
		()->GetCurrentSceneComponentsByType(Engine::ECS::ComponentTypes::RigidBody2D))
	{
		std::static_pointer_cast<Engine::ECS::RigidBody2DComponent>(rigidbody)->UpdateTransformComponent();
	}
}

b2Body* Engine::Physics::PhysicsManager::AddBody(const b2BodyDef& body)const
{
	return m_b2World->CreateBody(&body);
}
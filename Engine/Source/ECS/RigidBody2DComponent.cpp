#include "RigidBody2DComponent.h"
#include "../Physics/PhysicsManager.h"

Engine::ECS::RigidBody2DComponent::RigidBody2DComponent(const Engine::Utilities::UUID& uuid, BodyType type, const std::shared_ptr<TransformComponent>& transform, bool fixedRotation)
	:Component(ComponentTypes::RigidBody2D, "RigidBody2D",uuid), 
	m_transformComponent(transform),m_fixedRotation(fixedRotation)
{
	b2BodyDef bodyDef;
	bodyDef.position.Set(m_transformComponent->GetTranslation().x, m_transformComponent->GetTranslation().y);
	bodyDef.angle = m_transformComponent->GetRotation().z;
	switch (type) {
	case BodyType::Static:
		bodyDef.type = b2_staticBody;
		break;
	case BodyType::Dynamic:
		bodyDef.type = b2_dynamicBody;
		break;
	case BodyType::Kinematic:
		bodyDef.type = b2_kinematicBody;
		break;
	default:
		bodyDef.type = b2_staticBody;
		break;
	}

	m_rigidBody = Engine::Core::DependencyResolver::ResolveDependency<Engine::Physics::PhysicsManager>()->AddBody(bodyDef);
	m_rigidBody->SetFixedRotation(m_fixedRotation);
	b2PolygonShape boxShape;
	
	boxShape.SetAsBox(0.8f, 0.8f);
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.5f;
	fixtureDef.restitution = 0.5f;
	fixtureDef.restitutionThreshold = 0.5f;
	m_fixture = m_rigidBody->CreateFixture(&fixtureDef);
}

Engine::ECS::RigidBody2DComponent::RigidBody2DComponent(const RigidBody2DComponent& rhs)
	:Component(rhs.m_type, rhs.m_name, rhs.m_owningEntityUuid)
{
	m_fixedRotation = rhs.m_fixedRotation;
	m_transformComponent = rhs.m_transformComponent;
	m_fixture = rhs.m_fixture;
	m_rigidBody = rhs.m_rigidBody;
}

const std::string Engine::ECS::RigidBody2DComponent::Serialize()const
{
	std::stringstream ss;
	ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"BodyType\":" << m_rigidBody->GetType() << ",\"FixedRotation\":" << m_fixedRotation << "}";
	return  ss.str();
}

void Engine::ECS::RigidBody2DComponent::UpdateTransformComponent()
{
	m_transformComponent->GetTranslation().x = m_rigidBody->GetPosition().x;
	m_transformComponent->GetTranslation().y = m_rigidBody->GetPosition().y;
	m_transformComponent->GetRotation().z = m_rigidBody->GetAngle();
}

const int Engine::ECS::RigidBody2DComponent::GetBodyType() const
{
	return (int)m_rigidBody->GetType();
}

void Engine::ECS::RigidBody2DComponent::SetBodyType(BodyType type)
{
	switch (type) {
	case BodyType::Static:
		m_rigidBody->SetType(b2BodyType::b2_staticBody);
		break;
	case BodyType::Dynamic:
		m_rigidBody->SetType(b2BodyType::b2_dynamicBody);
		break;
	case BodyType::Kinematic:
		m_rigidBody->SetType(b2BodyType::b2_kinematicBody);
		break;
	default:
		m_rigidBody->SetType(b2BodyType::b2_staticBody);
		break;
	}
}

bool* Engine::ECS::RigidBody2DComponent::GetFixedRotation()
{
	return &m_fixedRotation;
}


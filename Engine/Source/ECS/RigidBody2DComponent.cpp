#include "RigidBody2DComponent.h"

Engine::ECS::RigidBody2DComponent::RigidBody2DComponent(BodyType type, bool fixedRotation)
	:Component(ComponentTypes::RigidBody2D, "RigidBody2D"), 
	m_bodyType(type),m_fixedRotation(fixedRotation)
{}

Engine::ECS::RigidBody2DComponent::RigidBody2DComponent(const RigidBody2DComponent& rhs)
	:Component(rhs.m_type, rhs.m_name)
{
	m_fixedRotation = rhs.m_fixedRotation;
	m_bodyType = rhs.m_bodyType;
}

const std::string Engine::ECS::RigidBody2DComponent::Serialize()const
{
	std::stringstream ss;
	ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"BodyType\":" << (int)m_bodyType << ",\"FixedRotation\":" << m_fixedRotation << "}";
	return  ss.str();
}

const Engine::ECS::BodyType Engine::ECS::RigidBody2DComponent::GetBodyType() const
{
	return m_bodyType;
}

void Engine::ECS::RigidBody2DComponent::SetRigidBody(b2Body* body)
{
	m_rigidBody = body;
}

void Engine::ECS::RigidBody2DComponent::SetBodyType(BodyType type)
{
	m_bodyType = type;
}

bool* Engine::ECS::RigidBody2DComponent::GetFixedRotation()
{
	return &m_fixedRotation;
}

b2Body* Engine::ECS::RigidBody2DComponent::GetBody() const
{
	return m_rigidBody;
}

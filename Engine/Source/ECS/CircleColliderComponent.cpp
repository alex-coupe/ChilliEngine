#include "CircleColliderComponent.h"

Engine::ECS::CircleColliderComponent::CircleColliderComponent(float radius, DirectX::XMFLOAT2 offset, float density, float friction, float restitution, float restThreshold)
	:Component(ComponentTypes::CircleCollider, "CircleCollider"),
	m_radius(radius),m_offset(offset),m_density(density),m_friction(friction),
	m_restitution(restitution),m_restitutionThreshold(restThreshold)
{
}

const std::string Engine::ECS::CircleColliderComponent::Serialize() const
{
	std::stringstream ss;
	ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"Radius\":" << m_radius 
		<< ", \"OffSetX\":" << m_offset.x << ", \"OffSetY\":" << m_offset.y
		<< ", \"Density\":" << m_density
		<< ", \"Friction\":" << m_friction << ", \"Restitution\":" << m_restitution
		<< ", \"RestitutionThreshold\":" << m_restitutionThreshold << "}";
	return  ss.str();
}

float& Engine::ECS::CircleColliderComponent::GetRadius()
{
	return m_radius;
}

DirectX::XMFLOAT2& Engine::ECS::CircleColliderComponent::GetOffset()
{
	return m_offset;
}

float& Engine::ECS::CircleColliderComponent::GetDensity()
{
	return m_density;
}

float& Engine::ECS::CircleColliderComponent::GetFriction()
{
	return m_friction;
}

float& Engine::ECS::CircleColliderComponent::GetRestitution()
{
	return m_restitution;
}

float& Engine::ECS::CircleColliderComponent::GetRestituitonThreshold()
{
	return m_restitutionThreshold;
}

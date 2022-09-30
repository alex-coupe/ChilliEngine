#include "CircleColliderComponent.h"

namespace Chilli {
	CircleColliderComponent::CircleColliderComponent(float radius, DirectX::XMFLOAT2 offset, float density, float friction, float restitution, float restThreshold)
		:Component(ComponentTypes::CircleCollider, "CircleCollider"),
		m_radius(radius), m_offset(offset), m_density(density), m_friction(friction),
		m_restitution(restitution), m_restitutionThreshold(restThreshold)
	{
	}

	CircleColliderComponent::CircleColliderComponent(CircleColliderComponent& rhs)
		:Component(rhs.m_type, rhs.m_name)
	{
		m_radius = rhs.m_radius;
		m_offset = rhs.m_offset;
		m_density = rhs.m_density;
		m_friction = rhs.m_friction;
		m_restitution = rhs.m_restitution;
		m_restitutionThreshold = rhs.m_restitutionThreshold;
	}

	const std::string CircleColliderComponent::Serialize() const
	{
		std::stringstream ss;
		ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"Radius\":" << m_radius
			<< ", \"OffSetX\":" << m_offset.x << ", \"OffSetY\":" << m_offset.y
			<< ", \"Density\":" << m_density
			<< ", \"Friction\":" << m_friction << ", \"Restitution\":" << m_restitution
			<< ", \"RestitutionThreshold\":" << m_restitutionThreshold << "}";
		return  ss.str();
	}

	float& CircleColliderComponent::GetRadius()
	{
		return m_radius;
	}

	DirectX::XMFLOAT2& CircleColliderComponent::GetOffset()
	{
		return m_offset;
	}

	float& CircleColliderComponent::GetDensity()
	{
		return m_density;
	}

	float& CircleColliderComponent::GetFriction()
	{
		return m_friction;
	}

	float& CircleColliderComponent::GetRestitution()
	{
		return m_restitution;
	}

	float& CircleColliderComponent::GetRestituitonThreshold()
	{
		return m_restitutionThreshold;
	}
}
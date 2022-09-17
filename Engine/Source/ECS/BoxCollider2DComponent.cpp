#include "BoxCollider2DComponent.h"

Engine::ECS::BoxCollider2DComponent::BoxCollider2DComponent(DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 offset, float density, float friction, float restitution, float restThreshold)
	:Component(ComponentTypes::BoxCollider2D, "BoxCollider2D"),
	m_size(size),m_offset(offset),m_density(density),m_friction(friction),m_restitution(restitution),
	m_restitutionThreshold(restThreshold)
{
}

const std::string Engine::ECS::BoxCollider2DComponent::Serialize() const
{
	std::stringstream ss;
	ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"SizeX\":" << m_size.x
		<< ", \"SizeY\":" << m_size.y <<  ", \"OffSetX\":" << m_offset.x
	<< ", \"OffSetY\":" << m_offset.y << ", \"Density\":" << m_density
	<< ", \"Friction\":" << m_friction << ", \"Restitution\":" << m_restitution 
		<< ", \"RestitutionThreshold\":" << m_restitutionThreshold << "}";
	return  ss.str();
}

DirectX::XMFLOAT2& Engine::ECS::BoxCollider2DComponent::GetSize() 
{
	return m_size;
}

DirectX::XMFLOAT2& Engine::ECS::BoxCollider2DComponent::GetOffset() 
{
	return m_offset;
}

float& Engine::ECS::BoxCollider2DComponent::GetDensity()
{
	return m_density;
}

float& Engine::ECS::BoxCollider2DComponent::GetFriction()
{
	return m_friction;
}

float& Engine::ECS::BoxCollider2DComponent::GetRestitution()
{
	return m_restitution;

}

float& Engine::ECS::BoxCollider2DComponent::GetRestituitonThreshold()
{
	return m_restitutionThreshold;
}

#include "BoxCollider2D.h"

Engine::ECS::BoxCollider2D::BoxCollider2D()
	:Component(ComponentTypes::BoxCollider2D,"BoxCollider2D")
{}

const std::string Engine::ECS::BoxCollider2D::Serialize() const
{
	std::stringstream ss;
	return  ss.str();
}

DirectX::XMFLOAT2& Engine::ECS::BoxCollider2D::GetSize() 
{
	return m_size;
}

DirectX::XMFLOAT2& Engine::ECS::BoxCollider2D::GetOffset() 
{
	return m_offset;
}

float& Engine::ECS::BoxCollider2D::GetDensity()
{
	return m_density;
}

float& Engine::ECS::BoxCollider2D::GetFriction()
{
	return m_friction;
}

float& Engine::ECS::BoxCollider2D::GetRestitution()
{
	return m_restitution;

}

float& Engine::ECS::BoxCollider2D::GetRestituitonThreshold()
{
	return m_restitutionThreshold;
}

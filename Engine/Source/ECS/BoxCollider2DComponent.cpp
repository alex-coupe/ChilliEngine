#include "BoxCollider2DComponent.h"
namespace Chilli {
	BoxCollider2DComponent::BoxCollider2DComponent(DirectX::XMFLOAT2 size, DirectX::XMFLOAT2 offset, float density, float friction, float restitution, float restThreshold)
		:Component(ComponentType::BoxCollider2D, "BoxCollider2D"),
		m_size(size), m_offset(offset), m_density(density), m_friction(friction), m_restitution(restitution),
		m_restitutionThreshold(restThreshold)
	{
	}

	BoxCollider2DComponent::BoxCollider2DComponent(BoxCollider2DComponent& rhs)
		:Component(rhs.m_type, rhs.m_name)
	{
		m_size = rhs.m_size;
		m_offset = rhs.m_offset;
		m_density = rhs.m_density;
		m_friction = rhs.m_friction;
		m_restitution = rhs.m_restitution;
		m_restitutionThreshold = rhs.m_restitutionThreshold;
	}

	void BoxCollider2DComponent::Clone(const std::shared_ptr<Component>& rhs)
	{
		const auto& comp = std::static_pointer_cast<BoxCollider2DComponent>(rhs);
		m_size = comp->m_size;
		m_offset = comp->m_offset;
		m_density = comp->m_density;
		m_friction = comp->m_friction;
		m_restitution = comp->m_restitution;
		m_restitutionThreshold = comp->m_restitutionThreshold;
	}

	const std::string BoxCollider2DComponent::Serialize(uint64_t entityId) const
	{
		std::stringstream ss;
		ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"SizeX\":" << m_size.x
			<< ", \"SizeY\":" << m_size.y << ", \"OffSetX\":" << m_offset.x
			<< ", \"OffSetY\":" << m_offset.y << ", \"Density\":" << m_density
			<< ", \"Friction\":" << m_friction << ", \"Restitution\":" << m_restitution
			<< ", \"RestitutionThreshold\":" << m_restitutionThreshold << "}";
		return  ss.str();
	}

	DirectX::XMFLOAT2& BoxCollider2DComponent::GetSize()
	{
		return m_size;
	}

	DirectX::XMFLOAT2& BoxCollider2DComponent::GetOffset()
	{
		return m_offset;
	}

	float& BoxCollider2DComponent::GetDensity()
	{
		return m_density;
	}

	float& BoxCollider2DComponent::GetFriction()
	{
		return m_friction;
	}

	float& BoxCollider2DComponent::GetRestitution()
	{
		return m_restitution;

	}

	float& BoxCollider2DComponent::GetRestituitonThreshold()
	{
		return m_restitutionThreshold;
	}
}
#include "RigidBody2DComponent.h"
namespace Chilli {

	RigidBody2DComponent::RigidBody2DComponent(BodyType type, bool fixedRotation)
		:Component(ComponentType::RigidBody2D, "RigidBody2D"),
		m_bodyType(type), m_fixedRotation(fixedRotation)
	{}

	RigidBody2DComponent::RigidBody2DComponent(const RigidBody2DComponent& rhs)
		:Component(rhs.m_type, rhs.m_name)
	{
		m_fixedRotation = rhs.m_fixedRotation;
		m_bodyType = rhs.m_bodyType;
	}

	void RigidBody2DComponent::Clone(const std::shared_ptr<Component>& rhs)
	{
		const auto comp = std::static_pointer_cast<RigidBody2DComponent>(rhs);
		m_bodyType = comp->m_bodyType;
		m_fixedRotation = comp->m_fixedRotation;
	}

	const std::string RigidBody2DComponent::Serialize(uint64_t entityId)const
	{
		std::stringstream ss;
		ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"BodyType\":" << (int)m_bodyType << ",\"FixedRotation\":" << m_fixedRotation << "}";
		return  ss.str();
	}

	const BodyType RigidBody2DComponent::GetBodyType() const
	{
		return m_bodyType;
	}

	void RigidBody2DComponent::SetRigidBody(b2Body* body)
	{
		m_rigidBody = body;
	}

	void RigidBody2DComponent::SetBodyType(BodyType type)
	{
		m_bodyType = type;
	}

	bool* RigidBody2DComponent::GetFixedRotation()
	{
		return &m_fixedRotation;
	}

	b2Body* RigidBody2DComponent::GetBody() const
	{
		return m_rigidBody;
	}
}

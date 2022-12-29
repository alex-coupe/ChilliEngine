#include "SpriteComponent.h"

namespace Chilli {
	SpriteComponent::SpriteComponent(bool transparent)
		:Component(ComponentType::Sprite, "Sprite"), m_transparent(transparent)
	{
	}

	SpriteComponent::SpriteComponent(SpriteComponent& rhs)
		:Component(rhs.m_type, rhs.m_name), m_transparent(rhs.m_transparent)
	{
	}

	void SpriteComponent::Clone(const std::shared_ptr<Component>& rhs)
	{
		const auto& comp = std::static_pointer_cast<SpriteComponent>(rhs);
		m_transparent = comp->m_transparent;
	}

	const std::string SpriteComponent::Serialize(uint64_t entityId) const
	{
		std::stringstream ss;
		ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"Transparent\":" << m_transparent
			<< "}";
		return  ss.str();
	}
}

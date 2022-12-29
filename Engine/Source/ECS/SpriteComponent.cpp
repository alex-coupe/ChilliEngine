#include "SpriteComponent.h"

namespace Chilli {
	SpriteComponent::SpriteComponent(bool transparent, uint64_t texId)
		:Component(ComponentType::Sprite, "Sprite"), m_transparent(transparent), m_texId(texId)
	{
	}

	SpriteComponent::SpriteComponent(SpriteComponent& rhs)
		:Component(rhs.m_type, rhs.m_name), m_transparent(rhs.m_transparent), m_texId(rhs.m_texId)
	{
	}

	bool& SpriteComponent::Transparent()
	{
		return m_transparent;
	}

	uint64_t SpriteComponent::GetTexId() const
	{
		return m_texId;
	}

	void SpriteComponent::SetTexId(uint64_t texId)
	{
		m_texId = texId;
	}

	void SpriteComponent::Clone(const std::shared_ptr<Component>& rhs)
	{
		const auto& comp = std::static_pointer_cast<SpriteComponent>(rhs);
		m_transparent = comp->m_transparent;
		m_texId = comp->m_texId;
	}

	const std::string SpriteComponent::Serialize(uint64_t entityId) const
	{
		auto boolText = m_transparent ? "true" : "false";
		std::stringstream ss;
		ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"Transparent\":" << boolText
		<< ",\"TextureId\":" << m_texId << "}";
		return  ss.str();
	}
}

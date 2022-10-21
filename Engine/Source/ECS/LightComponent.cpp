#include "LightComponent.h"

namespace Chilli {

	LightComponent::LightComponent(LightType type, DirectX::XMFLOAT3 col)
		:Component(ComponentType::Light,"Light"), m_lightType(type), m_color(col)
	{}

	LightComponent::LightComponent(LightComponent& rhs)
		:Component(rhs.m_type, rhs.m_name)
	{
		m_color = rhs.m_color;
		m_lightType = rhs.m_lightType;
	}

	LightType LightComponent::GetLightType()
	{
		return m_lightType;
	}

	void LightComponent::Clone(const std::shared_ptr<Component>& rhs)
	{
		const auto& comp = std::static_pointer_cast<LightComponent>(rhs);
		m_color = comp->m_color;
		m_lightType = comp->m_lightType;
	}

	const std::string LightComponent::Serialize(uint64_t entityId) const
	{
		std::stringstream ss;
		ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"LightType\":" << (int)m_lightType
			<< ", \"ColR\":" << m_color.x << ", \"ColG\":" << m_color.y << ", \"ColB\":" << m_color.z << "}";
		return  ss.str();
	}

	void LightComponent::SetType(LightType type)
	{
		m_lightType = type;
	}

	DirectX::XMFLOAT3& LightComponent::Color()
	{
		return m_color;
	}

}
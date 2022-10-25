#include "LightComponent.h"

namespace Chilli {

	LightComponent::LightComponent(LightType type, DirectX::XMFLOAT3 ambient, DirectX::XMFLOAT3 diffuse, DirectX::XMFLOAT3 specular)
		:Component(ComponentType::Light,"Light"), m_lightType(type)
	{
		m_ambient = ambient;
		m_diffuse = diffuse;
		m_specular = specular;
	}

	LightComponent::LightComponent(LightComponent& rhs)
		:Component(rhs.m_type, rhs.m_name)
	{
		m_ambient = rhs.m_ambient;
		m_diffuse = rhs.m_diffuse;
		m_specular = rhs.m_specular;
		m_lightType = rhs.m_lightType;
	}

	LightType LightComponent::GetLightType()
	{
		return m_lightType;
	}

	void LightComponent::Clone(const std::shared_ptr<Component>& rhs)
	{
		const auto& comp = std::static_pointer_cast<LightComponent>(rhs);
		m_ambient = comp->m_ambient;
		m_diffuse = comp->m_diffuse;
		m_specular = comp->m_specular;
		m_lightType = comp->m_lightType;
	}

	const std::string LightComponent::Serialize(uint64_t entityId) const
	{
		std::stringstream ss;
		ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"LightType\":" << (int)m_lightType
			<< ", \"AmbR\":" << m_ambient.x << ", \"AmbG\":" << m_ambient.y << ", \"AmbB\":" << m_ambient.z 
			<< ", \"DiffR\":" << m_diffuse.x << ", \"DiffG\":" << m_diffuse.y << ", \"DiffB\":" << m_diffuse.z
			<< ", \"SpecR\":" << m_specular.x << ", \"SpecG\":" << m_specular.y << ", \"SpecB\":" << m_specular.z
			<< "}";
		return  ss.str();
	}

	void LightComponent::SetType(LightType type)
	{
		m_lightType = type;
	}

	DirectX::XMFLOAT3& LightComponent::Ambient()
	{
		return m_ambient;
	}

	DirectX::XMFLOAT3& LightComponent::Diffuse()
	{
		return m_diffuse;
	}

	DirectX::XMFLOAT3& LightComponent::Specular()
	{
		return m_specular;
	}

}
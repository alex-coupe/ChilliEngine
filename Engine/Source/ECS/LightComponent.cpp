#include "LightComponent.h"

namespace Chilli {

	LightComponent::LightComponent(LightType type, DirectX::XMFLOAT3 ambient, DirectX::XMFLOAT3 diffuse, DirectX::XMFLOAT3 specular, float linear,float constant,float quadratic)
		:Component(ComponentType::Light,"Light"), m_lightType(type), m_ambient(ambient),m_diffuse(diffuse),
		m_specular(specular),m_linear(linear),m_constant(constant),m_quadratic(quadratic)
	{}

	LightComponent::LightComponent(LightComponent& rhs)
		:Component(rhs.m_type, rhs.m_name)
	{
		m_ambient = rhs.m_ambient;
		m_diffuse = rhs.m_diffuse;
		m_specular = rhs.m_specular;
		m_linear = rhs.m_linear;
		m_constant = rhs.m_constant;
		m_quadratic = rhs.m_quadratic;
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
		m_linear = comp->m_linear;
		m_constant = comp->m_constant;
		m_quadratic = comp->m_quadratic;
		m_lightType = comp->m_lightType;
	}

	const std::string LightComponent::Serialize(uint64_t entityId) const
	{
		std::stringstream ss;
		ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"LightType\":" << (int)m_lightType
			<< ", \"AmbR\":" << m_ambient.x << ", \"AmbG\":" << m_ambient.y << ", \"AmbB\":" << m_ambient.z 
			<< ", \"DiffR\":" << m_diffuse.x << ", \"DiffG\":" << m_diffuse.y << ", \"DiffB\":" << m_diffuse.z
			<< ", \"SpecR\":" << m_specular.x << ", \"SpecG\":" << m_specular.y << ", \"SpecB\":" << m_specular.z
			<< ", \"Linear\":" << m_linear << ", \"Quadratic\":" << m_quadratic << ", \"Constant\":" << m_constant
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

	float& LightComponent::Linear()
	{
		return m_linear;
	}

	float& LightComponent::Quadratic()
	{
		return m_quadratic;
	}

	float& LightComponent::Constant()
	{
		return m_constant;
	}
}
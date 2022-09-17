#include "TransformComponent.h"

Engine::ECS::TransformComponent::TransformComponent(DirectX::XMFLOAT3 translation, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale)
	:Component(ComponentTypes::Transform, "Transform"), m_translation(translation), m_rotation(rotation), m_scale(scale)
{
}

Engine::ECS::TransformComponent::TransformComponent(const TransformComponent& rhs)
	:Component(rhs.m_type,rhs.m_name)
{
	m_rotation = rhs.m_rotation;
	m_scale = rhs.m_scale;
	m_translation = rhs.m_translation;
}

DirectX::XMFLOAT3& Engine::ECS::TransformComponent::GetTranslation()
{
	return m_translation;
}

DirectX::XMFLOAT3& Engine::ECS::TransformComponent::GetRotation()
{
	return m_rotation;
}

DirectX::XMFLOAT3& Engine::ECS::TransformComponent::GetScale()
{
	return m_scale;
}

const std::string Engine::ECS::TransformComponent::Serialize()const
{

	std::stringstream ss;
	ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"PosX\":" << m_translation.x << ", \"PosY\":"<< m_translation.y 
		<< ", \"PosZ\":" << m_translation.z << ", \"RotX\":" << m_rotation.x << ", \"RotY\":" << m_rotation.y
		<< ",\"RotZ\":" << m_rotation.z << ", \"ScaleX\":" << m_scale.x << ", \"ScaleY\":" << m_scale.y
		<< ", \"ScaleZ\":" << m_scale.z << "}";
	return  ss.str();
}

DirectX::XMMATRIX Engine::ECS::TransformComponent::GetTransformMatrix() const
{
	return
		DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) *
		DirectX::XMMatrixTranslation(m_translation.x, m_translation.y, m_translation.z);
		
}

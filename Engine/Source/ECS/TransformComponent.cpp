#include "TransformComponent.h"
namespace Chilli {

	TransformComponent::TransformComponent(DirectX::XMFLOAT3 translation, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale)
		:Component(ComponentType::Transform, "Transform"), m_translation(translation), m_rotation(rotation), m_scale(scale)
	{
		auto rotMat = DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z);
		m_rotQuart = DirectX::XMQuaternionRotationMatrix(rotMat);
	}

	TransformComponent::TransformComponent(const TransformComponent& rhs)
		:Component(rhs.m_type, rhs.m_name)
	{
		m_rotation = rhs.m_rotation;
		m_scale = rhs.m_scale;
		m_translation = rhs.m_translation;
	}

	DirectX::XMFLOAT3& TransformComponent::Translation()
	{
		return m_translation;
	}

	DirectX::XMVECTOR& TransformComponent::RotQuart()
	{
		return m_rotQuart;
	}

	DirectX::XMFLOAT3& TransformComponent::Rotation()
	{
		return m_rotation;
	}

	DirectX::XMFLOAT3& TransformComponent::Scale()
	{
		return m_scale;
	}

	const std::string TransformComponent::Serialize(uint64_t entityId)const
	{

		std::stringstream ss;
		ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"PosX\":" << m_translation.x << ", \"PosY\":" << m_translation.y
			<< ", \"PosZ\":" << m_translation.z << ", \"RotX\":" << m_rotation.x << ", \"RotY\":" << m_rotation.y
			<< ",\"RotZ\":" << m_rotation.z << ", \"ScaleX\":" << m_scale.x << ", \"ScaleY\":" << m_scale.y
			<< ", \"ScaleZ\":" << m_scale.z << "}";
		return  ss.str();
	}

	void TransformComponent::Clone(const std::shared_ptr<Component>& rhs)
	{
		const auto& comp = std::static_pointer_cast<TransformComponent>(rhs);
		m_rotation = comp->m_rotation;
		m_scale = comp->m_scale;
		m_translation = comp->m_translation;
	}


	DirectX::XMMATRIX TransformComponent::GetTransformMatrix() const
	{
		return
			DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
			DirectX::XMMatrixRotationQuaternion(m_rotQuart) *
			DirectX::XMMatrixTranslation(m_translation.x, m_translation.y, m_translation.z);
	}
}
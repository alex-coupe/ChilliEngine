#include "TransformComponent.h"

Engine::ECS::TransformComponent::TransformComponent(DirectX::XMFLOAT3 translation, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale)
	:Component(ComponentTypes::Transform, "Transform"), m_translation(translation), m_rotation(rotation), m_scale(scale)
{
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

void Engine::ECS::TransformComponent::DrawGui() const
{
	
	ImGui::InputFloat3("Translation", m_translationRemote[0]);
	ImGui::InputFloat3("Rotation", m_rotationRemote[0]);
	ImGui::InputFloat3("Scale", m_scaleRemote[0]);
	
}

DirectX::XMMATRIX Engine::ECS::TransformComponent::GetTransformMatrix() const
{
	return DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) *
		DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
		DirectX::XMMatrixTranslation(m_translation.x, m_translation.y, m_translation.z);
}

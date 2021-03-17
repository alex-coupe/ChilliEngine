#include "TransformComponent.h"

Engine::ECS::TransformComponent::TransformComponent(DirectX::XMFLOAT3 translation, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale)
	:Component(ComponentTypes::Transform), m_translation(translation), m_rotation(rotation), m_scale(scale)
{
}

const std::string Engine::ECS::TransformComponent::Serialize()const
{

	
	return  "{ \"Type\": ,\n \"PosX\":";
}

void Engine::ECS::TransformComponent::DrawGui() const
{
	//ImGui::InputFloat3("Translation", m_translation);
	//ImGui::InputFloat3("Rotation", &m_rotation);
	//ImGui::InputFloat3("Scale", &m_scale);
	
}

DirectX::XMMATRIX Engine::ECS::TransformComponent::GetTransformMatrix() const
{
	return DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) *
		DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
		DirectX::XMMatrixTranslation(m_translation.x, m_translation.y, m_translation.z);
}

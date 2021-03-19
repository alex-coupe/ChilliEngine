#include "Camera.h"

Engine::Rendering::Camera::Camera(DirectX::XMFLOAT3 position, float screen_width, float screen_height)
	:m_position(position), m_direction({ 0.0f,0.0f,1.0f }), m_up({0.0f,1.0f,0.0f})
{
}

DirectX::XMMATRIX Engine::Rendering::Camera::GetViewMatrix()
{
	DirectX::FXMVECTOR viewPosition = DirectX::XMVectorSet(m_position.x, m_position.y, m_position.z,1.0f);
	DirectX::FXMVECTOR target = DirectX::XMVectorSet(m_position.x+m_direction.x, m_position.y+m_direction.y, m_position.z+m_direction.z, 1.0f);
	DirectX::FXMVECTOR up = DirectX::XMVectorSet(m_up.x, m_up.y, m_up.z, 1.0f);
	return DirectX::XMMatrixLookAtLH(viewPosition,target,up);
}

void Engine::Rendering::Camera::DrawGui()
{
	ImGui::Begin("Editor Camera");
	ImGui::SliderFloat3("Position", m_positionRemote[0],-80.0f,80.0f);
	ImGui::SliderFloat("Yaw", &m_yaw, -30.0f, 30.0f);
	ImGui::SliderFloat("Pitch", &m_pitch, -30.0f, 30.0f);
	ImGui::End();
}

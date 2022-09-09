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
	return DirectX::XMMatrixLookAtLH(viewPosition,target,up) * DirectX::XMMatrixRotationRollPitchYaw(m_pitch,m_yaw,0.0f);
}

void Engine::Rendering::Camera::UpdatePosition(DirectX::XMFLOAT3 delta)
{
	m_position.x += delta.x * m_speed;
	m_position.y += delta.y * m_speed;
	m_position.z += delta.z * m_speed;
}

void Engine::Rendering::Camera::UpdateRotation(float pitch, float yaw)
{
	float sensitivity = 0.01f;
	if (pitch < 0)
		m_pitch -= sensitivity;

	if (pitch > 0)
		m_pitch += sensitivity;
	
	if (yaw > 0)
		m_yaw += sensitivity;

	if (yaw < 0)
		m_yaw -= sensitivity;
}

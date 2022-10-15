#include "EditorCamera.h"

namespace Chilli {
	using namespace DirectX;
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		:m_fov(fov), m_aspectRatio(aspectRatio), m_nearClip(nearClip), m_farClip(farClip),
		Camera(XMMatrixPerspectiveLH(fov, m_aspectRatio, nearClip, farClip))
	{
		m_position = XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f);
		m_target = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
		m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
		m_viewMatrix = XMMatrixLookAtLH(m_position, m_position + m_target, m_up);
	}

	void EditorCamera::OnResize(const float newWidth, const float newHeight)
	{
		m_aspectRatio = newHeight / newWidth;
		m_projMatrix = DirectX::XMMatrixPerspectiveLH(m_fov, m_aspectRatio, m_nearClip, m_farClip);
	}

	const DirectX::XMMATRIX& EditorCamera::GetViewMatrix()const
	{
		return m_viewMatrix;
	}

	const DirectX::XMMATRIX EditorCamera::GetViewProjMatrix()const
	{
		return m_viewMatrix * m_projMatrix;
	}

	void EditorCamera::UpdatePosition(const Direction dir)
	{
		float camSpeed = m_speed * DependencyResolver::ResolveDependency<Timer>()->GetDeltaTime();
		switch (dir)
		{
		case Direction::Forward:
			m_position += camSpeed * m_target;
			break;
		case Direction::Backward:
			m_position -= camSpeed * m_target;
			break;
		case Direction::Left:
			m_position += XMVector3Normalize(XMVector3Cross(m_target, m_up)) * camSpeed;
			break;
		case Direction::Right:
			m_position -= XMVector3Normalize(XMVector3Cross(m_target, m_up)) * camSpeed;
			break;
		}
		m_viewMatrix = XMMatrixLookAtLH(m_position, m_position + m_target, m_up);
	}

	void EditorCamera::UpdateRotation(float yoffset, float xoffset)
	{
		
		xoffset *= m_sensitivity;
		yoffset *= m_sensitivity;

		m_yaw -= xoffset;
		m_pitch += yoffset;

		if (m_pitch > 89.0f)
			m_pitch = 89.0f;
		if (m_pitch < -89.0f)
			m_pitch = -89.0f;

		auto direction = XMVectorSet(
			cos(XMConvertToRadians(m_yaw)) * cos(XMConvertToRadians(m_pitch)),
			sin(XMConvertToRadians(m_pitch)), 
			sin(XMConvertToRadians(m_yaw)) * cos(XMConvertToRadians(m_pitch)), 1.0f);
		m_target = XMVector3Normalize(direction);
		m_viewMatrix = XMMatrixLookAtLH(m_position, m_position + m_target, m_up);
	}
}
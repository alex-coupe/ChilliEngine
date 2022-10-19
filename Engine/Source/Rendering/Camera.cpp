#include "Camera.h"
#include "../Gui/GuiManager.h"

namespace Chilli {
	using namespace DirectX;
	Camera::Camera(float fov, float aspectRatio, float nearClip, float farClip, CameraType camType, ProjectionType projType,XMFLOAT3 position)
		:m_fov(fov), m_aspectRatio(aspectRatio), m_nearClip(nearClip), m_farClip(farClip),m_cameraType(camType),m_projectionType(projType)
	{
		m_position = XMVectorSet(position.x, position.y, position.z, 1.0f);
		m_target = XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
		m_up = XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);
		m_viewMatrix = XMMatrixLookAtLH(m_position, m_position + m_target, m_up);
		
		if (m_projectionType == ProjectionType::Orthographic)
			m_viewMatrix = XMMatrixInverse(nullptr, m_viewMatrix);

		if (m_projectionType == ProjectionType::Perspective)
			m_projMatrix = XMMatrixPerspectiveLH(fov, m_aspectRatio, nearClip, farClip);
		else
			m_projMatrix = XMMatrixOrthographicLH(m_fov, m_aspectRatio,m_nearClip,m_farClip);
	}

	void Camera::OnResize(const float newWidth, const float newHeight)
	{
		m_aspectRatio = newHeight / newWidth;
		if (m_projectionType == ProjectionType::Perspective)
			m_projMatrix = XMMatrixPerspectiveLH(m_fov, m_aspectRatio, m_nearClip, m_farClip);
		else
			m_projMatrix = XMMatrixOrthographicLH(m_fov, m_aspectRatio, m_nearClip, m_farClip);
	}

	const DirectX::XMMATRIX& Camera::GetViewMatrix()const
	{
		return m_viewMatrix;
	}

	const DirectX::XMMATRIX& Camera::GetProjMatrix()const
	{
		return m_projMatrix;
	}

	const DirectX::XMMATRIX Camera::GetViewProjMatrix()const
	{
		return m_viewMatrix * m_projMatrix;
	}

	void Camera::UpdatePosition(const Direction dir)
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

	void Camera::UpdatePosition(const DirectX::XMFLOAT3 translation, const DirectX::XMFLOAT3 rotation)
	{
		m_position = XMVectorSet(translation.x, translation.y, translation.z, 1.0f);

		if (m_yRotOffset != rotation.y || m_xRotOffset != rotation.x)
		{
			m_xRotOffset = rotation.x;
			m_yaw -= m_xRotOffset * m_sensitivity;

			m_yRotOffset = rotation.y;
			m_pitch += m_yRotOffset * m_sensitivity;

			if (m_pitch > 89.0f)
				m_pitch = 89.0f;
			if (m_pitch < -89.0f)
				m_pitch = -89.0f;
			auto direction = XMVectorSet(
				cos(XMConvertToRadians(m_yaw)) * cos(XMConvertToRadians(m_pitch)),
				sin(XMConvertToRadians(m_pitch)),
				sin(XMConvertToRadians(m_yaw)) * cos(XMConvertToRadians(m_pitch)), 1.0f);
			m_target = XMVector3Normalize(direction);
		}
		m_viewMatrix = XMMatrixLookAtLH(m_position, m_position + m_target, m_up);
		if (m_projectionType == ProjectionType::Orthographic)
			m_viewMatrix = XMMatrixInverse(nullptr, m_viewMatrix);
	}

	void Camera::UpdateRotation(float yoffset, float xoffset)
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
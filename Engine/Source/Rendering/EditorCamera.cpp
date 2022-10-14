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

	
}
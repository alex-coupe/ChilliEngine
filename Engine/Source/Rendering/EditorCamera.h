#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Camera.h"

namespace Chilli {
	class CHILLI_API EditorCamera :public Camera {
	public:
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);
		virtual void OnResize(const float newWidth, const float newHeight)override;
		const DirectX::XMMATRIX& GetViewMatrix()const;
		const DirectX::XMMATRIX GetViewProjMatrix()const;
	private:
		float m_fov;
		float m_aspectRatio;
		float m_nearClip;
		float m_farClip;
		float m_pitch = 0.0f;
		float m_yaw = 0.0f;
		DirectX::XMVECTOR m_position;
		DirectX::XMVECTOR m_target;
		DirectX::XMVECTOR m_up;
		DirectX::XMMATRIX m_viewMatrix;
	};
}
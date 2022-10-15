#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Camera.h"
#include "../Core/Timer.h"

namespace Chilli {
	enum class Direction {
		Up,Down,Left,Right,Forward,Backward
	};
	class CHILLI_API EditorCamera :public Camera {
	public:
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);
		virtual void OnResize(const float newWidth, const float newHeight)override;
		const DirectX::XMMATRIX& GetViewMatrix()const;
		const DirectX::XMMATRIX GetViewProjMatrix()const;
		void UpdatePosition(const Direction dir);
		void UpdateRotation(float yoffset, float xoffset);
	private:
		float m_fov;
		float m_aspectRatio;
		float m_nearClip;
		float m_farClip;
		float m_pitch = 0.0f;
		float m_yaw = 90.0f;
		float m_speed = 10.0f;
		DirectX::XMVECTOR m_position;
		DirectX::XMVECTOR m_target;
		DirectX::XMVECTOR m_up;
		DirectX::XMMATRIX m_viewMatrix;
	};
}
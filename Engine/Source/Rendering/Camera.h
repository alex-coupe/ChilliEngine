#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Direct3D.h"

namespace Engine::Rendering {
	class CHILLI_API Camera {
	public:
		Camera(DirectX::XMFLOAT3 position, float screen_width, float screen_height);
		DirectX::XMMATRIX GetViewMatrix();
		void UpdatePosition(DirectX::XMFLOAT3 delta);
		void UpdateRotation(float pitch, float yaw);
	private:
		DirectX::XMFLOAT3 m_position;
		DirectX::XMFLOAT3 m_direction;
		DirectX::XMFLOAT3 m_up;
		float m_fov = 45.0f;
		float m_pitch = 0.0f;
		float m_yaw = 0.0f;
		float m_speed = 2.5f;		
	};
}
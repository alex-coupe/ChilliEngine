#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/DependencyResolver.h"
#include "../Core/Timer.h"
#include <DirectXMath.h>

namespace Chilli {
	enum class Direction {
		Up,Down,Left,Right,Forward,Backward
	};
	enum class CameraType {
		Editor, Scene
	};
	enum class ProjectionType {
		Perspective, Orthographic
	};
	class CHILLI_API Camera {
	public:
		Camera(float fov, float aspectRatio, float nearClip, float farClip, CameraType camType, ProjectionType projType, DirectX::XMFLOAT3 position = {0.0f,0.0f,-5.0f});
		void OnResize(const float newWidth, const float newHeight);
		const DirectX::XMMATRIX& GetViewMatrix()const;
		const DirectX::XMMATRIX GetViewProjMatrix()const;
		const DirectX::XMMATRIX& GetProjMatrix()const;
		void UpdatePosition(const Direction dir);
		void UpdatePosition(const DirectX::XMFLOAT3 translation, const DirectX::XMFLOAT3 rotation);
		void UpdateRotation(float yoffset, float xoffset);
	private:
		CameraType m_cameraType;
		ProjectionType m_projectionType;
		float m_sensitivity = 0.5f;
		float m_fov;
		float m_aspectRatio;
		float m_nearClip;
		float m_farClip;
		float m_pitch = 0.0f;
		float m_yaw = 90.0f;
		float m_xRotOffset = 0.0f;
		float m_yRotOffset = 0.0f;
		float m_speed = 15.0f;
		DirectX::XMVECTOR m_position;
		DirectX::XMVECTOR m_target;
		DirectX::XMVECTOR m_up;
		DirectX::XMMATRIX m_viewMatrix;
		DirectX::XMMATRIX m_projMatrix;
	};
}
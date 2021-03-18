#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include <DirectXMath.h>

namespace Engine::ECS {

	class CHILLI_API TransformComponent : public Component {
	public:
		TransformComponent(DirectX::XMFLOAT3 translation, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale);
		virtual const std::string Serialize()const override;
		virtual void DrawGui()const override;
		DirectX::XMMATRIX GetTransformMatrix()const;
	private:
		DirectX::XMFLOAT3 m_translation;
		DirectX::XMFLOAT3 m_rotation;
		DirectX::XMFLOAT3 m_scale;
		//For ImGui
		float* m_translationRemote[3] = {&m_translation.x, &m_translation.y, &m_translation.z};
		float* m_rotationRemote[3] = { &m_rotation.x, &m_rotation.y, &m_rotation.z };
		float* m_scaleRemote[3] = { &m_scale.x, &m_scale.y, &m_scale.z };
		
	};

}

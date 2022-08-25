#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include <DirectXMath.h>

namespace Engine::ECS {

	class CHILLI_API TransformComponent : public Component {
	public:
		TransformComponent(DirectX::XMFLOAT3 translation, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale);
		virtual const std::string Serialize()const override;
		DirectX::XMMATRIX GetTransformMatrix()const;
	private:
		DirectX::XMFLOAT3 m_translation;
		DirectX::XMFLOAT3 m_rotation;
		DirectX::XMFLOAT3 m_scale;		
	};

}

#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include <DirectXMath.h>

namespace Engine::ECS {

	class CHILLI_API TransformComponent : public Component {
	public:
		TransformComponent(DirectX::XMFLOAT3 translation = { 0.0f,0.0f,0.0f }, 
			DirectX::XMFLOAT3 rotation = {0.0f,0.0f,0.0f}, 
			DirectX::XMFLOAT3 scale = {1.0f,1.0f,1.0f});
		TransformComponent(const TransformComponent& rhs);
		virtual const std::string Serialize()const override;
		DirectX::XMMATRIX GetTransformMatrix()const;
		DirectX::XMFLOAT3& GetTranslation();
		DirectX::XMFLOAT3& GetRotation();
		DirectX::XMFLOAT3& GetScale();
	private:
		DirectX::XMFLOAT3 m_translation;
		DirectX::XMFLOAT3 m_rotation;
		DirectX::XMFLOAT3 m_scale;		
	};

}

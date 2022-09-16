#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include <DirectXMath.h>

namespace Engine::ECS {

	class CHILLI_API TransformComponent : public Component {
	public:
		TransformComponent(const Engine::Utilities::UUID&,DirectX::XMFLOAT3 translation, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale);
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

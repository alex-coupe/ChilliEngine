#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include <DirectXMath.h>

namespace Chilli {

	class CHILLI_API TransformComponent : public Component {
	public:
		TransformComponent(DirectX::XMFLOAT3 translation = { 0.0f,0.0f,0.0f }, 
			DirectX::XMFLOAT3 rotation = {0.0f,0.0f,0.0f}, 
			DirectX::XMFLOAT3 scale = {1.0f,1.0f,1.0f});
		TransformComponent(const TransformComponent& rhs);
		virtual const std::string Serialize(uint64_t entityId)const override;
		virtual void Clone(const std::shared_ptr<Component>& rhs) override;
		DirectX::XMMATRIX GetTransformMatrix()const;
		DirectX::XMFLOAT3& Translation();
		DirectX::XMFLOAT3& Rotation();
		DirectX::XMFLOAT3& Scale();
	private:
		DirectX::XMFLOAT3 m_translation;
		DirectX::XMFLOAT3 m_rotation;
		DirectX::XMFLOAT3 m_scale;		
	};

}

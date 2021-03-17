#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include <DirectXMath.h>

namespace Engine::ECS {

	class CHILLI_API TransformComponent : public Component {
	public:
		TransformComponent(float position[3], float rot[3], float scale[3]);
		virtual void Serialize(const std::string& filename)override;
		virtual void Deserialize(const std::string& filename)override;
		virtual void DrawGui()const override;
		DirectX::XMMATRIX GetTransformMatrix()const;
	private:
		DirectX::XMFLOAT3 m_translation;
		DirectX::XMFLOAT3 m_rotation;
		DirectX::XMFLOAT3 m_scale;
	};

}

#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include <DirectXMath.h>

namespace Engine::ECS {

	class CHILLI_API TransformComponent : public Component {
	public:
		TransformComponent(DirectX::XMVECTORF32 translation, DirectX::XMVECTORF32 rotation, DirectX::XMVECTORF32 scale);
		virtual void Serialize(const std::string& filename)override;
		virtual void Deserialize(const std::string& filename)override;
		virtual void DrawGui()const override;
		DirectX::XMMATRIX GetTransformMatrix()const;
	private:
		DirectX::XMVECTORF32 m_translation;
		DirectX::XMVECTORF32 m_rotation;
		DirectX::XMVECTORF32 m_scale;
		DirectX::XMMATRIX m_transformationMatrix;
	};

}

#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include <DirectXMath.h>

namespace Engine::ECS {

	class CHILLI_API TransformComponent : public Component {
	public:
		TransformComponent(DirectX::XMVECTORF32 translation, DirectX::XMVECTORF32 rotation, DirectX::XMVECTORF32 scale)
			: m_translation(translation), m_rotation(rotation), m_scale(scale)
		{
		
		}
		virtual void Serialize()override
		{

		}
		virtual void Deserialize()override
		{

		}
	
	private:
		DirectX::XMVECTORF32 m_translation;
		DirectX::XMVECTORF32 m_rotation;
		DirectX::XMVECTORF32 m_scale;
		DirectX::XMMATRIX m_transformationMatrix;
	};

}

#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include <DirectXMath.h>

namespace Engine::ECS {
	class CHILLI_API BoxCollider2D : public Component {
	public:
		BoxCollider2D();
		virtual const std::string Serialize()const override;
		DirectX::XMFLOAT2& GetSize();
		DirectX::XMFLOAT2& GetOffset();
		float& GetDensity();
		float& GetFriction();
		float& GetRestitution();
		float& GetRestituitonThreshold();
	private:
		DirectX::XMFLOAT2 m_offset = { 0.0f,0.0f };
		DirectX::XMFLOAT2 m_size = { 1.0f,1.0f };
		float m_density = 1.0f;
		float m_friction = 0.5f;
		float m_restitution = 0.0f;
		float m_restitutionThreshold = 0.5f;
	};
}
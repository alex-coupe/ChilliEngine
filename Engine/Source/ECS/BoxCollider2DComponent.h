#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include <DirectXMath.h>

namespace Engine::ECS {
	class CHILLI_API BoxCollider2DComponent : public Component {
	public:
		BoxCollider2DComponent(DirectX::XMFLOAT2 size = { 1.0f,1.0f },
			DirectX::XMFLOAT2 offset = { 0.0f,0.0f }, float density = 1.0f,float friction = 0.5f,
			float restitution = 0.0f, float restThreshold = 0.5f);
		BoxCollider2DComponent(BoxCollider2DComponent& rhs);
		virtual const std::string Serialize()const override;
		DirectX::XMFLOAT2& GetSize();
		DirectX::XMFLOAT2& GetOffset();
		float& GetDensity();
		float& GetFriction();
		float& GetRestitution();
		float& GetRestituitonThreshold();
	private:
		DirectX::XMFLOAT2 m_offset;
		DirectX::XMFLOAT2 m_size;
		float m_density;
		float m_friction;
		float m_restitution;
		float m_restitutionThreshold;
	};
}
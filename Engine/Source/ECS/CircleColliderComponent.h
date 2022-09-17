#pragma once
#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include <DirectXMath.h>

namespace Engine::ECS {
	class CHILLI_API CircleColliderComponent : public Component {
	public:
		CircleColliderComponent(float radius = 0.5f,DirectX::XMFLOAT2 offset = { 0.0f,0.0f }, float density = 1.0f, float friction = 0.5f,
			float restitution = 0.0f, float restThreshold = 0.5f);
		virtual const std::string Serialize()const override;
		float& GetRadius();
		DirectX::XMFLOAT2& GetOffset();
		float& GetDensity();
		float& GetFriction();
		float& GetRestitution();
		float& GetRestituitonThreshold();
	private:
		DirectX::XMFLOAT2 m_offset;
		float m_radius;
		float m_density;
		float m_friction;
		float m_restitution;
		float m_restitutionThreshold;
	};
}
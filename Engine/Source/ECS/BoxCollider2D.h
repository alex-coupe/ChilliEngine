#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include <DirectXMath.h>
#include "../Physics/PhysicsManager.h"

namespace Engine::ECS {
	class CHILLI_API BoxCollider2D : public Component {
	public:
		BoxCollider2D(const Engine::Utilities::UUID& uuid);
		virtual const std::string Serialize()const override;
	private:
		DirectX::XMFLOAT2 m_offset = { 0.0f,0.0f };
		DirectX::XMFLOAT2 m_size = { 0.5f,0.5f };
		float m_density = 1.0f;
		float m_fiction = 0.5f;
		float m_restitution = 0.0f;
		float m_restitutionThreshold = 0.5f;
	};
}
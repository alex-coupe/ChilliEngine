#pragma once
#include "../Core/ChilliDefinitions.h"
#include "../Core/DependencyResolver.h"
#include "Component.h"
#include "box2d/box2d.h"
#include "TransformComponent.h"

namespace Engine::ECS {

	enum class BodyType : int {
		Static,Kinematic,Dynamic
	};

	class CHILLI_API RigidBody2DComponent : public Component {
	public:
		RigidBody2DComponent(BodyType type = BodyType::Static, bool fixedRotation = false);
		RigidBody2DComponent(const RigidBody2DComponent& rhs);
		virtual const std::string Serialize()const override;
		const BodyType GetBodyType()const;
		void SetRigidBody(b2Body* body);
		void SetBodyType(BodyType type);
		bool* GetFixedRotation();
		b2Body* GetBody()const;
	private:
		b2Body* m_rigidBody = nullptr;
		bool m_fixedRotation = false;
		BodyType m_bodyType;
	};
}
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

	struct RigidBody2DOptions : ComponentVariables {
		BodyType type;
		bool fixedRotation = false;
	};

	class CHILLI_API RigidBody2DComponent : public Component {
	public:
		RigidBody2DComponent(const Engine::Utilities::UUID& uuid,BodyType type, 
			const std::shared_ptr<TransformComponent>& transform, bool fixedRotation);
		RigidBody2DComponent(const RigidBody2DComponent& rhs);
		virtual const std::string Serialize()const override;
		void UpdateTransformComponent();
		const int GetBodyType()const;
		void SetBodyType(BodyType type);
		bool* GetFixedRotation();
	private:
		b2Body* m_rigidBody = nullptr;
		b2Fixture* m_fixture = nullptr;
		bool m_fixedRotation = false;
		std::shared_ptr<TransformComponent> m_transformComponent;
	};
}
#pragma once
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "RigidBody2DComponent.h"
#include "BoxCollider2DComponent.h"
#include "CircleColliderComponent.h"

namespace Engine::ECS {
	class CHILLI_API ComponentFactory {
	public:
		static std::shared_ptr<MeshComponent> MakeMeshComponent();
		static std::shared_ptr<BoxCollider2DComponent> MakeBoxCollider2DComponent();
		static std::shared_ptr<CircleColliderComponent> MakeCircleColliderComponent();
		static std::shared_ptr<TransformComponent> MakeTransformComponent();
		static std::shared_ptr<RigidBody2DComponent> MakeRigidBody2DComponent();
	};

} 
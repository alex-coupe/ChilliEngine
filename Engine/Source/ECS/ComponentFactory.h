#pragma once
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "RigidBody2DComponent.h"
#include "BoxCollider2D.h"

namespace Engine::ECS {
	class CHILLI_API ComponentFactory {
	public:
		static std::shared_ptr<MeshComponent> MakeMeshComponent();
		static std::shared_ptr<BoxCollider2D> MakeBoxCollider2DComponent();
		static std::shared_ptr<TransformComponent> MakeTransformComponent(DirectX::XMFLOAT3 translation, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale);
		static std::shared_ptr<RigidBody2DComponent> MakeRigidBody2DComponent(BodyType type, bool fixedRotation);
	};

} 
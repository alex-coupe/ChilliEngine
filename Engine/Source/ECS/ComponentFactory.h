#pragma once
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "RigidBody2DComponent.h"

namespace Engine::ECS {
	class CHILLI_API ComponentFactory {
	public:
		static std::shared_ptr<MeshComponent> MakeMeshComponent(const Engine::Utilities::UUID& uuid);
		static std::shared_ptr<TransformComponent> MakeTransformComponent(const Engine::Utilities::UUID& uuid,DirectX::XMFLOAT3 translation, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale);
		static std::shared_ptr<RigidBody2DComponent> MakeRigidBody2DComponent(const Engine::Utilities::UUID& uuid,BodyType type, const std::shared_ptr<TransformComponent>& transform, bool fixedRotation);
	};

} 
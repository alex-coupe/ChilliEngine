#pragma once
#include "TransformComponent.h"
#include "MeshComponent.h"

namespace Engine::ECS {

	class ComponentFactory {
	public:
		static std::shared_ptr<MeshComponent> MakeMeshComponent();
		static std::shared_ptr<TransformComponent> MakeTransformComponent(DirectX::XMFLOAT3 translation, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale);
	};
} 
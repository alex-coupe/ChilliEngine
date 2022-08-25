#include "ComponentFactory.h"

std::shared_ptr<Engine::ECS::MeshComponent> Engine::ECS::ComponentFactory::MakeMeshComponent()
{
	return std::make_shared<MeshComponent>();
}

std::shared_ptr<Engine::ECS::TransformComponent> Engine::ECS::ComponentFactory::MakeTransformComponent(DirectX::XMFLOAT3 translation, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale)
{
	return std::make_shared<TransformComponent>(translation, rotation, scale);
}
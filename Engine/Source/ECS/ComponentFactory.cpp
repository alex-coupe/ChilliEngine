#include "ComponentFactory.h"

std::shared_ptr<Engine::ECS::MeshComponent> 
Engine::ECS::ComponentFactory::MakeMeshComponent()
{
	return std::make_shared<MeshComponent>("Mesh");
}

std::shared_ptr<Engine::ECS::BoxCollider2D> Engine::ECS::ComponentFactory::MakeBoxCollider2DComponent()
{
	return std::make_shared<BoxCollider2D>();
}

std::shared_ptr<Engine::ECS::TransformComponent> 
Engine::ECS::ComponentFactory::MakeTransformComponent(DirectX::XMFLOAT3 translation, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale)
{
	return std::make_shared<TransformComponent>(translation, rotation, scale);
}

std::shared_ptr<Engine::ECS::RigidBody2DComponent> 
Engine::ECS::ComponentFactory::MakeRigidBody2DComponent(BodyType type, bool fixedRotation)
{
	return std::make_shared<RigidBody2DComponent>(type, fixedRotation);
}
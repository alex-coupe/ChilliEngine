#include "ComponentFactory.h"

std::shared_ptr<Engine::ECS::MeshComponent> 
Engine::ECS::ComponentFactory::MakeMeshComponent()
{
	return std::make_shared<MeshComponent>("Mesh");
}

std::shared_ptr<Engine::ECS::BoxCollider2DComponent> Engine::ECS::ComponentFactory::MakeBoxCollider2DComponent()
{
	return std::make_shared<BoxCollider2DComponent>();
}

std::shared_ptr<Engine::ECS::CircleColliderComponent> Engine::ECS::ComponentFactory::MakeCircleColliderComponent()
{
	return std::make_shared<CircleColliderComponent>();
}

std::shared_ptr<Engine::ECS::TransformComponent>Engine::ECS::ComponentFactory::MakeTransformComponent()
{
	return std::make_shared<TransformComponent>();
}

std::shared_ptr<Engine::ECS::RigidBody2DComponent>Engine::ECS::ComponentFactory::MakeRigidBody2DComponent()
{
	return std::make_shared<RigidBody2DComponent>();
}
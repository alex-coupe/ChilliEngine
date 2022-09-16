#include "ComponentFactory.h"

std::shared_ptr<Engine::ECS::MeshComponent> 
Engine::ECS::ComponentFactory::MakeMeshComponent(const Engine::Utilities::UUID& uuid)
{
	return std::make_shared<MeshComponent>(uuid,"Mesh");
}

std::shared_ptr<Engine::ECS::TransformComponent> 
Engine::ECS::ComponentFactory::MakeTransformComponent(const Engine::Utilities::UUID& uuid ,DirectX::XMFLOAT3 translation, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 scale)
{
	return std::make_shared<TransformComponent>(uuid,translation, rotation, scale);
}

std::shared_ptr<Engine::ECS::RigidBody2DComponent> 
Engine::ECS::ComponentFactory::MakeRigidBody2DComponent(const Engine::Utilities::UUID& uuid,BodyType type, const std::shared_ptr<TransformComponent>& transform, bool fixedRotation)
{
	return std::make_shared<RigidBody2DComponent>(uuid,type, transform, fixedRotation);
}
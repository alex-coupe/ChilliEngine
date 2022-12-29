#include "ComponentFactory.h"
namespace Chilli {

	std::shared_ptr<MeshComponent> ComponentFactory::MakeMeshComponent()
	{
		return std::make_shared<MeshComponent>();
	}

	std::shared_ptr<BoxCollider2DComponent> ComponentFactory::MakeBoxCollider2DComponent()
	{
		return std::make_shared<BoxCollider2DComponent>();
	}

	std::shared_ptr<CircleColliderComponent> ComponentFactory::MakeCircleColliderComponent()
	{
		return std::make_shared<CircleColliderComponent>();
	}

	std::shared_ptr<TransformComponent> ComponentFactory::MakeTransformComponent()
	{
		return std::make_shared<TransformComponent>();
	}

	std::shared_ptr<RigidBody2DComponent> ComponentFactory::MakeRigidBody2DComponent()
	{
		return std::make_shared<RigidBody2DComponent>();
	}

	std::shared_ptr<ScriptComponent> ComponentFactory::MakeScriptComponent()
	{
		return std::make_shared<ScriptComponent>();
	}

	std::shared_ptr<CameraComponent> ComponentFactory::MakeCameraComponent()
	{
		return std::make_shared<CameraComponent>();
	}

	std::shared_ptr<LightComponent> ComponentFactory::MakeLightComponent()
	{
		return std::make_shared<LightComponent>();
	}

	std::shared_ptr<SpriteComponent> ComponentFactory::MakeSpriteComponent()
	{
		return std::make_shared<SpriteComponent>();
	}

	std::shared_ptr<IDComponent> ComponentFactory::MakeIDComponent(const std::string& name)
	{
		return std::make_shared<IDComponent>(name);
	}
}
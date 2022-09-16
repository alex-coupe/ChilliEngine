#include "CameraComponent.h"

Engine::ECS::CameraComponent::CameraComponent(const Engine::Utilities::UUID& uuid)
	:Component(ComponentTypes::Camera, "Camera", uuid)
{
}

const std::string Engine::ECS::CameraComponent::Serialize() const
{
	std::stringstream ss;
	return  ss.str();
}

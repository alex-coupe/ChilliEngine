#include "CameraComponent.h"

Engine::ECS::CameraComponent::CameraComponent()
	:Component(ComponentTypes::Camera, "Camera")
{
}

const std::string Engine::ECS::CameraComponent::Serialize() const
{
	std::stringstream ss;
	return  ss.str();
}

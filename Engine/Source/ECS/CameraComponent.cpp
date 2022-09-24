#include "CameraComponent.h"
namespace Chilli {
	CameraComponent::CameraComponent()
		:Component(ComponentTypes::Camera, "Camera")
	{
	}

	const std::string CameraComponent::Serialize() const
	{
		std::stringstream ss;
		return  ss.str();
	}
}

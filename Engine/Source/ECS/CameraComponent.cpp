#include "CameraComponent.h"
namespace Chilli {
	CameraComponent::CameraComponent()
		:Component(ComponentTypes::Camera, "Camera")
	{
	}

	const std::string CameraComponent::Serialize(uint64_t entityId) const
	{
		std::stringstream ss;
		return  ss.str();
	}
}

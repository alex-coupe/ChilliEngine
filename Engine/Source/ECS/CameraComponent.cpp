#include "CameraComponent.h"
namespace Chilli {
	CameraComponent::CameraComponent()
		:Component(ComponentType::Camera, "Camera")
	{
	}

	const std::string CameraComponent::Serialize(uint64_t entityId) const
	{
		std::stringstream ss;
		return  ss.str();
	}

	void CameraComponent::Clone(const std::shared_ptr<Component>& rhs)
	{
		const auto& comp = std::static_pointer_cast<CameraComponent>(rhs);
		m_cameraType = comp->m_cameraType;
	}
}

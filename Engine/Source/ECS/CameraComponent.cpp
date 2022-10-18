#include "CameraComponent.h"
namespace Chilli {
	CameraComponent::CameraComponent(float fov, float nearClip, float farClip)
		:Component(ComponentType::Camera, "Camera"), m_cameraType(CameraType::Scene), m_projectionType(ProjectionType::Perspective)
	{
	}

	CameraComponent::CameraComponent(CameraComponent& rhs)
		:Component(rhs.m_type,rhs.m_name)
	{
		m_cameraType = rhs.m_cameraType;
		m_fov = rhs.m_fov;
		m_farClip = rhs.m_farClip;
		m_nearClip = rhs.m_nearClip;
		m_projectionType = rhs.m_projectionType;
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

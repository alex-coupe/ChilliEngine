#include "CameraComponent.h"
namespace Chilli {
	CameraComponent::CameraComponent(ProjectionType projType,float fov, float nearClip, float farClip)
		:Component(ComponentType::Camera, "Camera"), m_projectionType(projType), m_fov(fov), m_nearClip(nearClip)
		,m_farClip(farClip)
	{
	}

	CameraComponent::CameraComponent(CameraComponent& rhs)
		:Component(rhs.m_type,rhs.m_name)
	{
		m_fov = rhs.m_fov;
		m_farClip = rhs.m_farClip;
		m_nearClip = rhs.m_nearClip;
		m_projectionType = rhs.m_projectionType;
	}

	const std::string CameraComponent::Serialize(uint64_t entityId) const
	{
		std::stringstream ss;
		ss << "{ \"Type\":" << static_cast<int>(m_type) << ", \"Fov\":" << m_fov
			<< ", \"FarClip\":" << m_farClip << ", \"NearClip\":" << m_nearClip
			<< ", \"ProjectionType\":" << (int)m_projectionType
			 << "}";
		return  ss.str();
	}

	void CameraComponent::Clone(const std::shared_ptr<Component>& rhs)
	{
		const auto& comp = std::static_pointer_cast<CameraComponent>(rhs);
		m_farClip = comp->m_farClip;
		m_fov = comp->m_fov;
		m_nearClip = comp->m_nearClip;
		m_projectionType = comp->m_projectionType;
	}

	float& CameraComponent::GetFov()
	{
		return m_fov;
	}

	float& CameraComponent::GetNearClip()
	{
		return m_nearClip;
	}

	float& CameraComponent::GetFarClip()
	{
		return m_farClip;
	}

	ProjectionType& CameraComponent::GetProjectionType()
	{
		return m_projectionType;
	}

	void CameraComponent::SetProjectionType(ProjectionType type)
	{
		m_projectionType = type;
	}
}

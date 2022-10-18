#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include "../Rendering/Camera.h"

namespace Chilli {

	class CHILLI_API CameraComponent : public Component {
	public:
		CameraComponent(float fov = 1.0f,float nearClip = 0.5f, float farClip = 100.0f);
		CameraComponent(CameraComponent& rhs);
		virtual const std::string Serialize(uint64_t entityId)const override;
		virtual void Clone(const std::shared_ptr<Component>& rhs)override;
	private:
		CameraType m_cameraType;
		ProjectionType m_projectionType;
		float m_fov;
		float m_nearClip;
		float m_farClip;
	};

}
#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include "../Rendering/Camera.h"

namespace Chilli {

	enum class CameraType : int {
		Perspective,Orthographic
	};

	class CHILLI_API CameraComponent : public Component {
	public:
		CameraComponent();
		virtual const std::string Serialize(uint64_t entityId)const override;
		virtual void Clone(const std::shared_ptr<Component>& rhs)override;
	private:
		CameraType m_cameraType;
		std::unique_ptr<Camera> m_camera;
	};

}
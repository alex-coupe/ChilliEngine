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

	private:
		CameraType m_type = CameraType::Perspective;
		std::unique_ptr<Camera> m_camera;
	};

}
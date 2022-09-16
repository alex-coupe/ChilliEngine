#pragma once
#include "../Core/ChilliDefinitions.h"
#include "Component.h"
#include "../Rendering/Camera.h"

namespace Engine::ECS {
	enum class CameraType : int {
		Perspective,Orthographic
	};
	class CHILLI_API CameraComponent : public Component {
	public:
		CameraComponent(const Engine::Utilities::UUID& uuid);
		virtual const std::string Serialize()const override;

	private:
		CameraType m_type = CameraType::Perspective;
		std::unique_ptr<Engine::Rendering::Camera> m_camera;
	};

}
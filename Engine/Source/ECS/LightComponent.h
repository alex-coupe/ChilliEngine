#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include "../Rendering/Light.h"

namespace Chilli {
	class CHILLI_API LightComponent : public Component {
	public:
		LightComponent(LightType type = LightType::Directional, DirectX::XMFLOAT3 col = { 1.0f,1.0f,1.0f });
		LightComponent(LightComponent& rhs);
		virtual const std::string Serialize(uint64_t entityId)const override;
		virtual void Clone(const std::shared_ptr<Component>& rhs)override;
		LightType GetLightType();
		void SetType(LightType type);
		DirectX::XMFLOAT3& Color();
	private:
		DirectX::XMFLOAT3 m_color;
		LightType m_lightType;
	};
}
#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include "../Rendering/Light.h"

namespace Chilli {

	class CHILLI_API LightComponent : public Component {
	public:
		LightComponent(LightType type = LightType::Directional);
		LightComponent(LightComponent& rhs);
		virtual const std::string Serialize(uint64_t entityId)const override;
		virtual void Clone(const std::shared_ptr<Component>& rhs)override;
		LightType GetLightType();
		void SetType(LightType type);
		DirectX::XMFLOAT3& Ambient();
		DirectX::XMFLOAT3& Diffuse();
		DirectX::XMFLOAT3& Specular();
	private:
		DirectX::XMFLOAT3 m_ambient;
		DirectX::XMFLOAT3 m_diffuse;
		DirectX::XMFLOAT3 m_specular;
		LightType m_lightType;
	};
}
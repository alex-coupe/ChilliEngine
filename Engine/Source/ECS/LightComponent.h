#pragma once
#include "Component.h"
#include <DirectXMath.h>
#include "../Rendering/Light.h"

namespace Chilli {

	class CHILLI_API LightComponent : public Component {
	public:
		LightComponent(LightType type = LightType::Directional, DirectX::XMFLOAT3 ambient = { 0.2f, 0.2f, 0.2f }, DirectX::XMFLOAT3 diffuse = { 0.5f, 0.5f, 0.5f }, DirectX::XMFLOAT3 specular ={ 1.0f, 1.0f, 1.0f },
			float linear = 0.09f, float constant = 1.0f,float quadratic = 0.032f);
		LightComponent(LightComponent& rhs);
		virtual const std::string Serialize(uint64_t entityId)const override;
		virtual void Clone(const std::shared_ptr<Component>& rhs)override;
		LightType GetLightType();
		void SetType(LightType type);
		DirectX::XMFLOAT3& Ambient();
		DirectX::XMFLOAT3& Diffuse();
		DirectX::XMFLOAT3& Specular();
		float& Linear();
		float& Constant();
		float& Quadratic();
	private:
		DirectX::XMFLOAT3 m_ambient;
		DirectX::XMFLOAT3 m_diffuse;
		DirectX::XMFLOAT3 m_specular;
		float m_linear;
		float m_constant;
		float m_quadratic;
		LightType m_lightType;
	};
}
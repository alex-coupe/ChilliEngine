#pragma once
#include "Light.h"

namespace Chilli {

	struct PointLightData {
		alignas(16)DirectX::XMFLOAT3 position;
		alignas(16)DirectX::XMFLOAT3 ambient;
		alignas(16)DirectX::XMFLOAT3 diffuse;
		alignas(16)DirectX::XMFLOAT3 specular;
		float linear;
		float quadratic;
		float constant;
	};

	class CHILLI_API PointLight : public Light{
	public:
		PointLight(Entity& entity);
		virtual void Update() override;
		PointLightData data;		
	};
}
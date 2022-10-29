#pragma once
#include "Light.h"

namespace Chilli {
	struct SpotlightData {
		alignas(16)DirectX::XMFLOAT3 position;
		alignas(16)DirectX::XMFLOAT3 direction;
		alignas(16)DirectX::XMFLOAT3 ambient;
		alignas(16)DirectX::XMFLOAT3 diffuse;
		alignas(16)DirectX::XMFLOAT3 specular;
		float linear;
		float quadratic;
		float constant;
		float cutOff;
		float outerCutOff;
	};

	class CHILLI_API Spotlight : public Light {
	public:
		Spotlight(Entity& entity);
		virtual void Update() override;
		SpotlightData data;
	};
}
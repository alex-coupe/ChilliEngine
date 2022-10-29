#pragma once
#include "Light.h"

namespace Chilli {

	struct DirectionalLightData {
		alignas(16)DirectX::XMFLOAT3 direction;
		alignas(16)DirectX::XMFLOAT3 ambient;
		alignas(16)DirectX::XMFLOAT3 diffuse;
		alignas(16)DirectX::XMFLOAT3 specular;
	};

	class CHILLI_API DirectionalLight : public Light {
	public:
		DirectionalLight(Entity& entity);
		virtual void Update() override;
		DirectionalLightData data;
		
	};
}
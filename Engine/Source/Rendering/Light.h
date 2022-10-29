#pragma once
#include "../Core/ChilliDefinitions.h"
#include <DirectXMath.h>
#include "../ResourceSystem/Mesh.h"
#include "../Rendering/Texture.h"

namespace Chilli {
	enum class LightType {
		DirectionalLight, PointLight, Spotlight
	};

	struct PointLightData {
		alignas(16)DirectX::XMFLOAT3 position;
		alignas(16)DirectX::XMFLOAT3 ambient;
		alignas(16)DirectX::XMFLOAT3 diffuse;
		alignas(16)DirectX::XMFLOAT3 specular;
		float linear;
		float quadratic;
		float constant;
	};

	struct DirectionalLightData {
		alignas(16)DirectX::XMFLOAT3 direction;
		alignas(16)DirectX::XMFLOAT3 ambient;
		alignas(16)DirectX::XMFLOAT3 diffuse;
		alignas(16)DirectX::XMFLOAT3 specular;
	};

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
	class CHILLI_API Light {
	public:
		Light(LightType type, Entity& ent);
		void Update();
		LightType GetLightType();
		PointLightData pointLightData = {};
		DirectionalLightData dirLightData = {};
		SpotlightData spotlightData = {};
	private:
		LightType m_lightType;
		UUID m_entityId;
	};
}
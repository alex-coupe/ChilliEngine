#pragma once
#include "../Core/ChilliDefinitions.h"
#include <DirectXMath.h>
#include "../ResourceSystem/Mesh.h"
#include "../Rendering/Texture.h"

namespace Chilli {
	enum class LightType {
		Directional, Point, Spotlight
	};
	struct LightProperties {
		alignas(16)DirectX::XMFLOAT3 position;
		alignas(16)DirectX::XMFLOAT3 ambient;
		alignas(16)DirectX::XMFLOAT3 diffuse;
		alignas(16)DirectX::XMFLOAT3 specular;
		float linear;
		float quadratic;
		float constant;
		float padding;
	};
	class CHILLI_API Light {
	public:
		Light(LightType type, Entity& entity);
		LightProperties properties;
		void Update();
	private:
		LightType m_type;
		UUID m_entityId;
	};
}
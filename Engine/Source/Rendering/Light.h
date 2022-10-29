#pragma once
#include "../Core/ChilliDefinitions.h"
#include <DirectXMath.h>
#include "../ResourceSystem/Mesh.h"
#include "../Rendering/Texture.h"

namespace Chilli {
	enum class LightType {
		DirectionalLight, PointLight, Spotlight
	};
	class CHILLI_API Light {
	public:
		Light(LightType type, UUID entityId) 
			:m_lightType(type), m_entityId(entityId)
		{}
		virtual void Update() = 0;
		LightType GetLightType()
		{
			return m_lightType;
		}
	protected:
		LightType m_lightType;
		UUID m_entityId;
	};
}
#pragma once
#include "../Core/ChilliDefinitions.h"
#include <DirectXMath.h>
#include "../ResourceSystem/Mesh.h"
#include "../Rendering/Texture.h"

namespace Chilli {
	enum class LightType {
		Directional, Point, Spotlight
	};
	class CHILLI_API Light {
	public:
		Light(LightType type, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir, DirectX::XMFLOAT3 col, UUID entId);
		DirectX::XMFLOAT3& GetPosition();
		DirectX::XMFLOAT3& GetColor();
	private:
		DirectX::XMFLOAT3 m_position;
		DirectX::XMFLOAT3 m_direction;
		DirectX::XMFLOAT3 m_color;
		LightType m_type;
		UUID m_entityId;
	};
}
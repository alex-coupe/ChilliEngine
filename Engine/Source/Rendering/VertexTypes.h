#pragma once
#include <DirectXMath.h>

namespace Chilli {

	struct VertexPosColor {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 color;
	};

	struct VertexPos {
		DirectX::XMFLOAT3 position;
	};

	struct VertexPosTex {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 tex;
	};

	struct VertexPosTexNorm {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 tex;
		DirectX::XMFLOAT3 normals;
	};
}
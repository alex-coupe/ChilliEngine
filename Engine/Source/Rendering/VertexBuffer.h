#pragma once
#include "../Core/ChilliDefinitions.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <vector>

namespace Engine::Rendering {

	class VertexBuffer {
	public:
		template <typename T>
		VertexBuffer(const std::vector<T>& vertices)
		{
			D3D11_BUFFER_DESC vertex_buffer = {};

			vertex_buffer.Usage = D3D11_USAGE_DEFAULT;
			vertex_buffer.ByteWidth = sizeof(T) * sizeof(vertices);
			vertex_buffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertex_buffer.CPUAccessFlags = 0u;
			vertex_buffer.MiscFlags = 0u;
			vertex_buffer.StructureByteStride = sizeof(T);
		}
		void Bind();
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	};

}
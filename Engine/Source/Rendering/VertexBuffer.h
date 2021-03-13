#pragma once
#include "../Core/ChilliDefinitions.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "Direct3D.h"

namespace Engine::Rendering {

	class VertexBuffer {
	public:
		template <typename T>
		VertexBuffer(const std::vector<T>& vertices, const std::shared_ptr<Direct3D>& d3d)
			:m_direct3d(d3d), m_stride(sizeof(T))
		{
			D3D11_BUFFER_DESC vertex_buffer = {};

			vertex_buffer.Usage = D3D11_USAGE_DEFAULT;
			vertex_buffer.ByteWidth = m_stride * sizeof(vertices);
			vertex_buffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertex_buffer.CPUAccessFlags = 0u;
			vertex_buffer.MiscFlags = 0u;
			vertex_buffer.StructureByteStride = m_stride;

			D3D11_SUBRESOURCE_DATA buffer_data = {};

			buffer_data.pSysMem = vertices.data();

			if (FAILED(m_hresult = m_direct3d->GetDevice()->CreateBuffer(&vertex_buffer, &buffer_data, &m_buffer)))
			{
				m_direct3d->GetDXError();
			}

		}
		void Bind();
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		std::shared_ptr<Direct3D> m_direct3d;
		HRESULT m_hresult = 0;
		UINT m_stride;
	};

}
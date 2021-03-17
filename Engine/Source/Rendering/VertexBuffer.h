#pragma once
#include "../Core/ChilliDefinitions.h"
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
			vertex_buffer.ByteWidth = m_stride * static_cast<UINT>(vertices.size());
			vertex_buffer.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertex_buffer.CPUAccessFlags = 0u;
			vertex_buffer.MiscFlags = 0u;
			vertex_buffer.StructureByteStride = m_stride;

			D3D11_SUBRESOURCE_DATA buffer_data = {};

			buffer_data.pSysMem = vertices.data();

			GFX_THROW_ERR(m_direct3d->GetDevice()->CreateBuffer(&vertex_buffer, &buffer_data, &m_buffer));

		}
		void Bind();
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		std::shared_ptr<Direct3D> m_direct3d;
		UINT m_stride;
		HRESULT m_result = 0;
	};

}
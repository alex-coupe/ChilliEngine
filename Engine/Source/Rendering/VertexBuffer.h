#pragma once
#include "../Core/ChilliDefinitions.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include "Renderer.h"

namespace Engine::Rendering {

	class VertexBuffer {
	public:
		template <typename T>
		VertexBuffer(const std::vector<T>& vertices, std::shared_ptr<Renderer> renderer)
			:m_renderer(renderer), m_stride(sizeof(T))
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

			if (FAILED(m_hresult = m_renderer->GetDevice()->CreateBuffer(&vertex_buffer, &buffer_data, &m_buffer)))
			{
				m_renderer->GetDXError();
			}

		}
		void Bind();
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
		std::shared_ptr<Renderer> m_renderer;
		HRESULT m_hresult = 0;
		UINT m_stride;
	};

}